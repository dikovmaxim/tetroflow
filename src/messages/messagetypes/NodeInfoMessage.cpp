#include "NodeInfoMessage.hpp"
#include "../NetworkMessage.hpp"
#include "../MessageUtils.hpp"

#include <cstring>

NodeInfoMessage::NodeInfoMessage(std::shared_ptr<ClusterNode> thisNode) : thisNode(thisNode) {}
NodeInfoMessage::NodeInfoMessage() : thisNode(nullptr) {}

MessageType NodeInfoMessage::getType() {
    return MessageType::NOTIFY_CLUSTER;
}

std::vector<std::byte> NodeInfoMessage::serialize() {
    std::vector<std::byte> bytes;
    //packet format: clusterNodeID, state, iplen, ip, port
    //clusterNodeID
    uint64_t clusterNodeID = thisNode->getClusterNodeID();
    std::byte* clusterNodeIDBytes = (std::byte*)&clusterNodeID;
    bytes.insert(bytes.end(), clusterNodeIDBytes, clusterNodeIDBytes + sizeof(uint64_t));

    //state
    ClusterNodeState state = thisNode->getState();
    std::byte* stateBytes = (std::byte*)&state;
    bytes.insert(bytes.end(), stateBytes, stateBytes + sizeof(ClusterNodeState));

    //ip length
    uint8_t iplen = thisNode->getIP().size();
    std::byte* iplenBytes = (std::byte*)&iplen;
    bytes.insert(bytes.end(), iplenBytes, iplenBytes + sizeof(uint8_t));

    //ip
    std::string ip = thisNode->getIP();
    for (char c : ip) {
        bytes.push_back(std::byte(c));
    }

    //port
    uint16_t port = thisNode->getPort();
    std::byte* portBytes = (std::byte*)&port;
    bytes.insert(bytes.end(), portBytes, portBytes + sizeof(uint16_t));

    return bytes;
}


void NodeInfoMessage::deserialize(const std::vector<std::byte>& bytes) {
    //packet format: clusterNodeID, state, iplen, ip, port
    //clusterNodeID

    int offset = 0;

    uint64_t clusterNodeID = *((uint64_t*)&bytes[offset]);
    offset += sizeof(uint64_t);

    ClusterNodeState state = *((ClusterNodeState*)&bytes[offset]);
    offset += sizeof(ClusterNodeState);

    uint8_t iplen = *((uint8_t*)&bytes[offset]);
    offset += sizeof(uint8_t);

    std::string ip;
    for (int i = 0; i < iplen; i++) {
        ip += (char)bytes[offset + i];
    }
    offset += iplen;

    uint16_t port = *((uint16_t*)&bytes[offset]);
    offset += sizeof(uint16_t);

    thisNode = std::make_shared<ClusterNode>(clusterNodeID, state, ip, port);
}


std::vector<std::byte> NodeInfoMessageToBytes(std::shared_ptr<NodeInfoMessage> message) {
    std::shared_ptr<NetworkMessage> networkMessage = createNetworkMessage(
        generate_unique_id(),
        generate_unique_timestamp(),
        0,
        message->getType(),
        message->serialize()
    );

    std::vector<std::byte> bytes;
    int offset = 0;

    //id
    uint64_t id = networkMessage->id;
    std::byte* idBytes = (std::byte*)&id;
    bytes.insert(bytes.end(), idBytes, idBytes + sizeof(uint64_t));

    //timestamp
    uint64_t timestamp = networkMessage->timestamp;
    std::byte* timestampBytes = (std::byte*)&timestamp;
    bytes.insert(bytes.end(), timestampBytes, timestampBytes + sizeof(uint64_t));

    //sender
    uint64_t sender = networkMessage->sender;
    std::byte* senderBytes = (std::byte*)&sender;
    bytes.insert(bytes.end(), senderBytes, senderBytes + sizeof(uint64_t));
    
    //length
    uint64_t length = networkMessage->length;
    std::byte* lengthBytes = (std::byte*)&length;
    bytes.insert(bytes.end(), lengthBytes, lengthBytes + sizeof(uint64_t));

    //checksum
    uint16_t checksum = networkMessage->checksum;
    std::byte* checksumBytes = (std::byte*)&checksum;
    bytes.insert(bytes.end(), checksumBytes, checksumBytes + sizeof(uint16_t));

    //type
    uint8_t type = networkMessage->type;
    std::byte* typeBytes = (std::byte*)&type;
    bytes.insert(bytes.end(), typeBytes, typeBytes + sizeof(uint8_t));

    //data
    for (int i = 0; i < networkMessage->length; i++) {
        bytes.push_back(std::byte(networkMessage->data[i]));
    }

    return bytes;
}


std::shared_ptr<NodeInfoMessage> NodeInfoMessageFromRawBytes(char* bytes, int length) {
    std::vector<std::byte> byteVector;
    for (int i = 0; i < length; i++) {
        byteVector.push_back(std::byte(bytes[i]));
    }

    //create a node info message
    std::shared_ptr<NodeInfoMessage> message = std::make_shared<NodeInfoMessage>();
    message->deserialize(byteVector);

    return message;
}