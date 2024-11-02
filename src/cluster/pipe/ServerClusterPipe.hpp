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
    void sendBytes(std::shared_ptr<std::vector<std::byte>> message) override;
    void closePipe() override;
private:
    void HandleMessagesWorker() override;
};