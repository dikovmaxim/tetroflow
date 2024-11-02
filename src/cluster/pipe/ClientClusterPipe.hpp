#pragma once

#include "ClusterPipe.hpp"

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <cstdint>
#include <thread>

class ClientClusterPipe : public ClusterPipe {
public:
    ClientClusterPipe(std::string ip, int port);
    void sendBytes(std::shared_ptr<std::vector<std::byte>> message) override;
    void closePipe() override;
private:
    void HandleMessagesWorker() override;
    void Connect() override;
    std::string ip;
    int port;
};