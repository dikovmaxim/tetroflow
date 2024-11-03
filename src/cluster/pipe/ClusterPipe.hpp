#pragma once

#include "../../messages/Message.hpp"

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <cstdint>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>


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

    virtual void closePipe() = 0;

    virtual void setOnMessage(std::function<void(std::shared_ptr<std::vector<std::byte>>)> onMessage);

    void setOnStateChange(std::function<void(ClusterPipeState)> onStateChange);
    void setState(ClusterPipeState state);
    ClusterPipeState getState();

    void addMessageToQueue(std::shared_ptr<Message> value);
    
protected:
    virtual void sendBytes(std::vector<std::byte> message) = 0;
    ClusterPipeState state;
    std::function<void(std::shared_ptr<std::vector<std::byte>>)> onMessage;
    std::function<void(ClusterPipeState)> onStateChange;
    int fdsock;
    std::thread workerThread;
    bool running;
    virtual void HandleMessagesWorker() = 0;
    virtual void Connect() = 0;

private:
    std::queue<std::shared_ptr<Message>> queue; // The queue for data
    std::mutex mtx;                             // Mutex for synchronization
    std::condition_variable cv;                 // Condition variable for notifying the worker

    void processQueue();                        // The worker function
    std::thread queueWorkerThread;              // The worker thread
};