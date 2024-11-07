#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>

#include "../ClusterNode.hpp"
#include "../Cluster.hpp"

std::mutex messagePoolMutex;
std::condition_variable messagePoolCV;

void AddMessageToPool(std::shared_ptr<Message> message) {
    std::lock_guard<std::mutex> lock(messagePoolMutex);
    messagePool.push_back(message);
    messagePoolCV.notify_one();
}

bool IsMessageInPool(std::shared_ptr<Message> message) {
    std::lock_guard<std::mutex> lock(messagePoolMutex);
    for (auto msg : messagePool) {
        if (msg == message) {
            return true;
        }
    }
    return false;
}

