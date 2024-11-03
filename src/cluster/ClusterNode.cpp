#include "ClusterNode.hpp"

#include <iostream>

ClusterNode::ClusterNode(uint64_t clusterNodeID, ClusterNodeState state, std::string ip, uint16_t port) {
    this->clusterNodeID = clusterNodeID;
    this->state = state;
    this->ip = ip;
    this->port = port;
}

uint64_t ClusterNode::getClusterNodeID() {
    return this->clusterNodeID;
}

ClusterNodeState ClusterNode::getState() {
    return this->state;
}

std::string ClusterNode::getIP() {
    return this->ip;
}

uint16_t ClusterNode::getPort() {
    return this->port;
}

void ClusterNode::InitializePipe(std::shared_ptr<ClusterPipe> pipe) {
    this->pipe = pipe;
    this->pipe->setOnStateChange([this](ClusterPipeState state) {
        this->HandlePipeStateChange(state);
    });
    this->pipe->setOnMessage([this](std::shared_ptr<std::vector<std::byte>> message) {
        this->HandlePipeMessage(message);
    });
}

void ClusterNode::HandlePipeStateChange(ClusterPipeState state) {
    if (state == ClusterPipeState::CONNECTED) {
        this->state = ClusterNodeState::ALIVE;
    } else if (state == ClusterPipeState::DISCONNECTED) {
        this->state = ClusterNodeState::DEAD;
    }

    std::cout << "Node " << this->clusterNodeID << " state changed to " << (int)this->state << std::endl;
}

void ClusterNode::HandlePipeMessage(std::shared_ptr<std::vector<std::byte>> message) {
    //print message
    std::cout << "Message received from node " << this->clusterNodeID << std::endl;
    for (auto byte : *message) {
        std::cout << (char)byte;
    }
    std::cout << std::endl;
}