#pragma once

#include "../Node.hpp"

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

class ServerNode : public Node {
public:
    ServerNode(int server_socket);

    void addMessageToQueue(std::shared_ptr<Message> message) override;
    void start() override;
    void stop() override;

    std::vector<std::shared_ptr<Message>> getReceivedMessages() override;
    std::vector<std::shared_ptr<Message>> getSentMessages() override;
    void sendNode(std::shared_ptr<Message> message) override;
    NodeStatus getStatus() const override;
    NodeType getType() const override;

private:
    std::chrono::time_point<std::chrono::system_clock> lastHeartbeat;

    NodeStatus status;

    int socket_fd;

    void setLastHeartbeat(int time) override;
    void sendJSONMessage(nlohmann::json message) override;
    void handleQueue() override;
    void nodeConnect() override;
    void nodeDisconnect() override;

    std::vector<std::shared_ptr<Message>> SentMessages;
    std::vector<std::shared_ptr<Message>> ReceivedMessages;
    std::queue<std::shared_ptr<Message>> messageQueue;

    std::thread messageQueueThread;
    std::thread incomingMessageThread;
};