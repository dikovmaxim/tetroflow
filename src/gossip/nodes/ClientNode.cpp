#include "ClientNode.hpp"

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


ClientNode::ClientNode(std::string ip, int port) {
    this->ip = ip;
    this->port = port;
    this->lastHeartbeat = std::chrono::system_clock::now();
}

std::mutex clientNodeMessageQueueMutex; // Mutex for protecting the message queue
std::condition_variable clientNodeMessageCvQueue; // Condition variable for message queue
bool clientNodeStopMessageQueueHandling = false; // Flag to stop message handling

void ClientNode::addMessageToQueue(std::shared_ptr<Message> message) {
    {
        std::lock_guard<std::mutex> lock(clientNodeMessageQueueMutex);
        messageQueue.push(message);
    }
    clientNodeMessageCvQueue.notify_one(); // Notify the handling thread
}

//handle the message queue
void ClientNode::handleQueue() {
    while (!clientNodeStopMessageQueueHandling) {
        std::shared_ptr<Message> message;
        {
            std::unique_lock<std::mutex> lock(clientNodeMessageQueueMutex);
            clientNodeMessageCvQueue.wait(lock, [this] { return !messageQueue.empty() || clientNodeStopMessageQueueHandling; });
            if (clientNodeStopMessageQueueHandling) {
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

void ClientNode::nodeConnect() {
    struct sockaddr_in serv_addr;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        log(LOG_ERROR, "Error creating socket for client node");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);

    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        log(LOG_ERROR, "Connection failed for client node");
        return;
    }

    incomingMessageThread = std::thread(&ClientNode::handleIncomingMessages, this);
    incomingMessageThread.detach();

    status = NodeStatus::ALIVE;
}

void ClientNode::nodeDisconnect() {
    close(socket_fd);
    status = NodeStatus::DEAD;
    removeNode(*this);
}

void ClientNode::sendJSONMessage(nlohmann::json message) {
    std::string messageString = message.dump();
    send(socket_fd, messageString.c_str(), messageString.length(), 0);
}

void ClientNode::sendNode(std::shared_ptr<Message> message) {
    nlohmann::json jsonMessage = message->ToJson();
    sendJSONMessage(jsonMessage);
    SentMessages.push_back(message);
}

void ClientNode::handleIncomingMessages() {
    char buffer[2048] = {0};
    while (true) {
        int valread = read(socket_fd, buffer, 1024);
        if (valread == 0 || valread < 0) {
            log(LOG_INFO, "Client node disconnected");
            this->status = NodeStatus::DEAD;
            nodeDisconnect();
            break;
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

void ClientNode::start() {
    nodeConnect();
    messageQueueThread = std::thread(&ClientNode::handleQueue, this);
    

    messageQueueThread.detach();
    
}

void ClientNode::stop() {
    clientNodeStopMessageQueueHandling = true;
    clientNodeMessageCvQueue.notify_one();
    nodeDisconnect();
}

std::vector<std::shared_ptr<Message>> ClientNode::getReceivedMessages() {
    return ReceivedMessages;
}

std::vector<std::shared_ptr<Message>> ClientNode::getSentMessages() {
    return SentMessages;
}

NodeStatus ClientNode::getStatus() const {
    return status;
}

NodeType ClientNode::getType() const {
    return NodeType::CLIENT;
}

void ClientNode::setLastHeartbeat(int time) {
    this->lastHeartbeat = std::chrono::system_clock::now();
}

std::string ClientNode::toString() const {
    return "Client node at " + ip + ":" + std::to_string(port);
}

bool ClientNode::compare(const Node& other) const {
    if (other.getType() != NodeType::CLIENT) {
        return false;
    }
    const ClientNode& otherClient = dynamic_cast<const ClientNode&>(other);
    return ip == otherClient.ip && port == otherClient.port;
}