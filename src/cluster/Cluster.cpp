#include "Cluster.hpp"
#include "ClusterNode.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>

///// Message Pool /////
std::vector<std::shared_ptr<Message>> messagePool;

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

///// Cluster management /////

std::vector<std::shared_ptr<ClusterNode>> clusterNodes;
std::vector<std::shared_ptr<ClusterNode>> activeNodes;

//move a node from the clusterNodes to the activeNodes
void ActivateNode(std::shared_ptr<ClusterNode> node) {
    std::lock_guard<std::mutex> lock(messagePoolMutex);
    clusterNodes.erase(std::remove(clusterNodes.begin(), clusterNodes.end(), node), clusterNodes.end());
    activeNodes.push_back(node);
}

//move a node from the activeNodes to the clusterNodes
void DeactivateNode(std::shared_ptr<ClusterNode> node) {
    std::lock_guard<std::mutex> lock(messagePoolMutex);
    activeNodes.erase(std::remove(activeNodes.begin(), activeNodes.end(), node), activeNodes.end());
    clusterNodes.push_back(node);
}

//add a node to the clusterNodes
void AddNode(std::shared_ptr<ClusterNode> node) {
    std::lock_guard<std::mutex> lock(messagePoolMutex);
    clusterNodes.push_back(node);
}

