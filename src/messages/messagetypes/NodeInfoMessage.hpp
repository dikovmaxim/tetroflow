#pragma once

#include <memory>
#include <vector>
#include <string>

#include "../Message.hpp"
#include "../MessageTypes.hpp"
#include "../../cluster/ClusterNode.hpp"

class NodeInfoMessage : public Message {
public:
    NodeInfoMessage(std::shared_ptr<ClusterNode> thisNode);
    NodeInfoMessage();
    MessageType getType() override;
    std::vector<std::byte> serialize() override;
    void deserialize(const std::vector<std::byte>& bytes) override;

    std::string toString() {
        return std::string("NodeInfoMessage { clusterNodeID: ") + std::to_string(thisNode->getClusterNodeID()) + ", state: " + std::to_string((int)thisNode->getState()) + ", ip: " + thisNode->getIP() + ", port: " + std::to_string(thisNode->getPort()) + " }";
    }
private:
    std::shared_ptr<ClusterNode> thisNode;
};

std::vector<std::byte> NodeInfoMessageToBytes(std::shared_ptr<NodeInfoMessage> message);
std::shared_ptr<NodeInfoMessage> NodeInfoMessageFromRawBytes(char* bytes, int length);