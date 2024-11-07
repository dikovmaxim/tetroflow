#pragma once

#include "./pipe/ClusterPipe.hpp"
#include "../messages/Message.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

enum class ClusterNodeState {
    UNKNOWN = 0,
    ALIVE = 1,
    DEAD = 2,
};

class ClusterNode {

protected:
    uint64_t lastHeartbeat;
    uint64_t clusterNodeID;
    ClusterNodeState state;
    std::string ip;
    uint16_t port;

    std::shared_ptr<ClusterPipe> pipe;

    void HandlePipeStateChange(ClusterPipeState state);
    void HandlePipeMessage(std::shared_ptr<std::vector<std::byte>> message);
    
    std::vector<std::shared_ptr<Message>> messagesSent;
    std::vector<std::shared_ptr<Message>> messagesReceived;

public:

    ClusterNode(uint64_t clusterNodeID, ClusterNodeState state, std::string ip, uint16_t port);
    ClusterNode();

    uint64_t getClusterNodeID();
    ClusterNodeState getState();
    std::string getIP();
    uint16_t getPort();

    void InitializePipe(std::shared_ptr<ClusterPipe> pipe);

};