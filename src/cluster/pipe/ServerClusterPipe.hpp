#pragma once

#include "ClusterPipe.hpp"

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <cstdint>
#include <thread>

class ServerClusterPipe : public ClusterPipe {
public:
    ServerClusterPipe(int sockfd);
    void closePipe() override;
private:
    void HandleMessagesWorker() override;
    void sendBytes(std::vector<std::byte> message) override;
};