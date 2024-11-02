#pragma once

#include <cstdint>
#include <string>

enum class ClusterNodeState {
    UNKNOWN = 0,
    ALIVE = 1,
    DEAD = 2,
};

class ClusterNode {

    uint64_t lastHeartbeat;
    uint64_t clusterNodeID;
    ClusterNodeState state;
    std::string ip;
    uint16_t port;

public:

    ClusterNode(uint64_t clusterNodeID, ClusterNodeState state, std::string ip, uint16_t port) : clusterNodeID(clusterNodeID), state(state), ip(ip), port(port) {
        lastHeartbeat = 0;
    }

    uint64_t getClusterNodeID() {
        return clusterNodeID;
    }

    ClusterNodeState getState() {
        return state;
    }

    std::string getIP() {
        return ip;
    }

    uint16_t getPort() {
        return port;
    }

};