#pragma once

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <queue>

#include "../messages/Message.hpp"

//node used for gossip protocol implementation

enum NodeStatus {
    ALIVE,
    SUSPECT,
    DEAD
};

class Node {
public:
    Node(std::string ip, int port);
    ~Node();

    void addMessageToQueue(std::shared_ptr<Message> message);
    void start();
    void stop();

    std::vector<std::shared_ptr<Message>> getReceivedMessages();
    std::vector<std::shared_ptr<Message>> getSentMessages();
    void send(std::shared_ptr<Message> message);

private:
    std::string ip;
    int port;
    std::chrono::time_point<std::chrono::system_clock> lastHeartbeat;

    void setLastHeartbeat(int time);

    std::vector<std::shared_ptr<Message>> SentMessages;
    std::vector<std::shared_ptr<Message>> ReceivedMessages;
    std::queue<std::shared_ptr<Message>> messageQueue;

    std::thread messageQueueThread;
    std::thread incomingMessageThread;
};