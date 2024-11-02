#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <cstdint>
#include <thread>

enum class ClusterPipeType {
    CLIENT,
    SERVER
};

enum class ClusterPipeState {
    CONNECTED,
    DISCONNECTED
};

class ClusterPipe {
public:
    virtual void sendBytes(std::shared_ptr<std::vector<std::byte>> message) = 0;
    virtual void closePipe() = 0;

    virtual void setOnMessage(std::function<void(std::shared_ptr<std::vector<std::byte>>)> onMessage);

    void setOnStateChange(std::function<void(ClusterPipeState)> onStateChange);
    void setState(ClusterPipeState state);
    ClusterPipeState getState();
    
protected:
    ClusterPipeState state;
    std::function<void(std::shared_ptr<std::vector<std::byte>>)> onMessage;
    std::function<void(ClusterPipeState)> onStateChange;
    int fdsock;
    std::thread workerThread;
    bool running;
    virtual void HandleMessagesWorker() = 0;
    virtual void Connect() = 0;
};