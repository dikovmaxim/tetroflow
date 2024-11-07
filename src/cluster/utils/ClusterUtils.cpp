#include "../ClusterNode.hpp"
#include "../Cluster.hpp"

bool NodeExists(std::shared_ptr<ClusterNode> node) {
    for (auto n : nodes) {
        if (n == node) {
            return true;
        }
    }
    return false;
}

//move a node from the clusterNodes to the activeNodes
void AddNode(std::shared_ptr<ClusterNode> node) {
    if (!NodeExists(node)) {
        nodes.push_back(node);
    }
}

//remove a node from the activeNodes
void RemoveNode(std::shared_ptr<ClusterNode> node) {
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i] == node) {
            nodes.erase(nodes.begin() + i);
            break;
        }
    }
}

