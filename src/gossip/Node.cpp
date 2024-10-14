#include "Node.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>


#include "../server/json_fwd.hpp"


Node::Node(std::string ip, int port) {
    this->ip = ip;
    this->port = port;
    this->lastHeartbeat = std::chrono::system_clock::now();
}

Node::~Node() {
    stop();
}

//send a message
void Node::sendNode(std::shared_ptr<Message> message) {
    //send the message
    sendJSONMessage(message->ToJson());
    //add the message to the sent messages
    SentMessages.push_back(message);
}

//send a json message
void Node::sendJSONMessage(nlohmann::json message) {
    //convert the json message to a string
    std::string msg = message.dump();
    //send the message
    send(socket_fd, msg.c_str(), msg.length(), 0);
}

//try to connect to the node by ip and port
void Node::start() {
    //create a socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    //create a sockaddr_in struct
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

    //connect to the server
    if (connect(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return;
    }

    //start the threads
    messageQueueThread = std::thread(&Node::handleQueue, this);
    incomingMessageThread = std::thread(&Node::handleIncomingMessages, this);
    //detach the threads
    messageQueueThread.detach();
    incomingMessageThread.detach();
}

//stop the threads
void Node::stop() {
    messageQueueThread.~thread();
    incomingMessageThread.~thread();
    //close the socket
    close(socket_fd);
}

//add a message to the queue
std::mutex messageQueueMutex; // Mutex for protecting the message queue
std::condition_variable cvQueue; // Condition variable for message queue
bool stopMessageQueueHandling = false; // Flag to stop message handling

void Node::addMessageToQueue(std::shared_ptr<Message> message) {
    {
        std::lock_guard<std::mutex> lock(messageQueueMutex);
        messageQueue.push(message);
    }
    cvQueue.notify_one(); // Notify the handling thread
}

//handle the message queue
void Node::handleQueue() {
    while (!stopMessageQueueHandling) {
        std::shared_ptr<Message> message;
        {
            std::unique_lock<std::mutex> lock(messageQueueMutex);
            cvQueue.wait(lock, [this] { return !messageQueue.empty() || stopMessageQueueHandling; });
            if (stopMessageQueueHandling) {
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

//handle incoming messages
void Node::handleIncomingMessages() {
    while (!stopMessageQueueHandling) {
        //read from the socket
        char buffer[1024] = {0};
        int valread = read(socket_fd, buffer, 1024);
        if (valread == 0) {
            this->status = NodeStatus::DEAD;
            return;
        }

        //print the message
        std::cout << buffer << std::endl;


    }
}



//get the received messages
std::vector<std::shared_ptr<Message>> Node::getReceivedMessages() {
    return ReceivedMessages;
}

//get the sent messages
std::vector<std::shared_ptr<Message>> Node::getSentMessages() {
    return SentMessages;
}

//get the status of the node
NodeStatus Node::getStatus() const {
    return status;
}

//set the last heartbeat time
void Node::setLastHeartbeat(int time) {
    lastHeartbeat = std::chrono::system_clock::now();
}

