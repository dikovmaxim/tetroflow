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

class Node {
public:
    Node(std::string ip, int port);
    ~Node();

    void addMessageToQueue(std::shared_ptr<Message> message);
    void start();
    void stop();

    std::vector<std::shared_ptr<Message>> getReceivedMessages();
    std::vector<std::shared_ptr<Message>> getSentMessages();
    void sendNode(std::shared_ptr<Message> message);
    NodeStatus getStatus() const;

private:
    std::string ip;
    int port;
    std::chrono::time_point<std::chrono::system_clock> lastHeartbeat;

    NodeStatus status;

    int socket_fd;

    void setLastHeartbeat(int time);
    void sendJSONMessage(nlohmann::json message);
    void handleQueue();
    void handleIncomingMessages();
    void nodeConnect();
    void nodeDisconnect();

    std::vector<std::shared_ptr<Message>> SentMessages;
    std::vector<std::shared_ptr<Message>> ReceivedMessages;
    std::queue<std::shared_ptr<Message>> messageQueue;

    std::thread messageQueueThread;
    std::thread incomingMessageThread;
};