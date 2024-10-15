#pragma once

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <queue>

#include "../messages/Message.hpp"
#include "../server/json_fwd.hpp"

//node used for gossip protocol implementation

enum NodeStatus {
    ALIVE,
    SUSPECT,
    DEAD
};

enum NodeType {
    CLIENT,
    SERVER
};

class Node {
public:

    virtual void addMessageToQueue(std::shared_ptr<Message> message) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual std::vector<std::shared_ptr<Message>> getReceivedMessages() = 0;
    virtual std::vector<std::shared_ptr<Message>> getSentMessages() = 0;
    virtual void sendNode(std::shared_ptr<Message> message) = 0;
    virtual NodeStatus getStatus() const = 0;
    virtual NodeType getType() const = 0;

private:
    std::chrono::time_point<std::chrono::system_clock> lastHeartbeat;

    NodeStatus status;

    int socket_fd;

    virtual void setLastHeartbeat(int time) = 0;
    virtual void sendJSONMessage(nlohmann::json message) = 0;
    virtual void handleQueue() = 0;
    virtual void nodeConnect() = 0;
    virtual void nodeDisconnect() = 0;

    std::vector<std::shared_ptr<Message>> SentMessages;
    std::vector<std::shared_ptr<Message>> ReceivedMessages;
    std::queue<std::shared_ptr<Message>> messageQueue;

    std::thread messageQueueThread;
    std::thread incomingMessageThread;
};