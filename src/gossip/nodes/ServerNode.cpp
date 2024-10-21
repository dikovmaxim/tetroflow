#include "ServerNode.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "../GossipManager.hpp"
#include "../../messages/Message.hpp"
#include "../../messages/MessageParser.hpp"
#include "../../messages/MessageHandler.hpp"
#include "../../log/Logger.hpp"


//this node is created when a new client connects to the server

ServerNode::ServerNode(int server_socket) {
    this->socket_fd = server_socket;
    this->lastHeartbeat = std::chrono::system_clock::now();
}

std::mutex serverNodeMessageQueueMutex; // Mutex for protecting the message queue
std::condition_variable serverNodeCvQueue; // Condition variable for message queue
bool serverNodeStopMessageQueueHandling = false; // Flag to stop message handling

void ServerNode::addMessageToQueue(std::shared_ptr<Message> message) {
    {
        std::lock_guard<std::mutex> lock(serverNodeMessageQueueMutex);
        messageQueue.push(message);
    }
    serverNodeCvQueue.notify_one(); // Notify the handling thread
}

//handle the message queue
void ServerNode::handleQueue() {
    while (!serverNodeStopMessageQueueHandling) {
        std::shared_ptr<Message> message;
        {
            std::unique_lock<std::mutex> lock(serverNodeMessageQueueMutex);
            serverNodeCvQueue.wait(lock, [this] { return !messageQueue.empty() || serverNodeStopMessageQueueHandling; });
            if (serverNodeStopMessageQueueHandling) {
                return;
            }
            message = messageQueue.front();
            messageQueue.pop();
        }
        if (message) {
            sendNode(message);
        }
    }
}

//scince this is a server node, we don't need to connect to anything
//we just start listening for incoming messages on the socket
void ServerNode::nodeConnect() {
    while (this->status == NodeStatus::ALIVE) {
        char buffer[2048] = {0};
        int valread = read(socket_fd, buffer, 1024);
        if (valread == 0) {
            log(LOG_INFO, "Server node disconnected");
            this->status = NodeStatus::DEAD;

            //call a destructor
            nodeDisconnect();

            return;
        }

        try
        {

            nlohmann::json j = nlohmann::json::parse(buffer);
            std::shared_ptr<Message> message = parseMessage(j);

            ReceivedMessages.push_back(message);

            addMessageToExchangeQueue(message);

        } catch (const std::exception& e) {}
    }
}

void ServerNode::nodeDisconnect() {
    close(socket_fd);
    removeNode(*this);
    this->status = NodeStatus::DEAD;
}

void ServerNode::sendJSONMessage(nlohmann::json message) {
    std::string jsonString = message.dump();
    ssize_t bytesSent = write(socket_fd, jsonString.c_str(), jsonString.size());
}


void ServerNode::start() {
    std::thread incomingMessageHandler(&ServerNode::nodeConnect, this);
    incomingMessageHandler.detach();

    std::thread messageQueueHandler(&ServerNode::handleQueue, this);
    messageQueueHandler.detach();

    this->status = NodeStatus::ALIVE;
}

void ServerNode::stop() {
    this->status = NodeStatus::DEAD;
    serverNodeStopMessageQueueHandling = true;
    serverNodeCvQueue.notify_one();
}

std::vector<std::shared_ptr<Message>> ServerNode::getReceivedMessages() {
    return ReceivedMessages;
}

std::vector<std::shared_ptr<Message>> ServerNode::getSentMessages() {
    return SentMessages;
}

NodeStatus ServerNode::getStatus() const {
    return status;
}

NodeType ServerNode::getType() const {
    return NodeType::SERVER;
}

void ServerNode::setLastHeartbeat(int time) {
    this->lastHeartbeat = std::chrono::system_clock::now();
}

//send a message to the client
void ServerNode::sendNode(std::shared_ptr<Message> message) {
    nlohmann::json jsonMessage = message->ToJson();
    sendJSONMessage(jsonMessage);
    SentMessages.push_back(message);
}

std::string ServerNode::toString() const {
    return "ServerNode " + std::to_string(socket_fd);
}

bool ServerNode::compare(const Node& other) const {
    if(other.getType() != NodeType::SERVER) {
        return false;
    }
    const ServerNode& otherServerNode = dynamic_cast<const ServerNode&>(other);
    return this->socket_fd == otherServerNode.socket_fd;
}