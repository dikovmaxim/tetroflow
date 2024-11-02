#include "NetworkMessage.hpp"
#include "MessageTypes.hpp"
#include "Message.hpp"
#include "messagetypes/NodeInfoMessage.hpp"


#include <memory>
#include <vector>
#include <stdexcept>
#include <iostream>


uint16_t calculateChecksum(std::shared_ptr<NetworkMessage> message) {
    uint16_t checksum = 0;
    //FIXME: implement checksum calculation
    return checksum;
}

std::shared_ptr<NetworkMessage> createNetworkMessage(uint64_t id, uint64_t timestamp, uint64_t sender, MessageType type, std::vector<std::byte> data) {
    std::shared_ptr<NetworkMessage> message = std::make_shared<NetworkMessage>();
    message->id = id;
    message->timestamp = timestamp;
    message->sender = sender;
    message->length = data.size();
    message->type = (uint8_t)type;
    message->data = new char[data.size()];
    for (int i = 0; i < data.size(); i++) {
        message->data[i] = (char)data[i];
    }
    message->checksum = calculateChecksum(message);
    return message;
}

std::shared_ptr<NetworkMessage> networkMessageFromBytes(std::vector<std::byte> bytes) {
    std::shared_ptr<NetworkMessage> message = std::make_shared<NetworkMessage>();
    int offset = 0;
    message->id = *((uint64_t*)&bytes[offset]);
    offset += sizeof(uint64_t);
    message->timestamp = *((uint64_t*)&bytes[offset]);
    offset += sizeof(uint64_t);
    message->sender = *((uint64_t*)&bytes[offset]);
    offset += sizeof(uint64_t);
    message->length = *((uint64_t*)&bytes[offset]);
    offset += sizeof(uint64_t);
    message->checksum = *((uint16_t*)&bytes[offset]);
    offset += sizeof(uint16_t);
    message->type = *((uint8_t*)&bytes[offset]);
    offset += sizeof(uint8_t);
    message->data = new char[message->length];
    for (int i = 0; i < message->length; i++) {
        message->data[i] = (char)bytes[offset + i];
    }
    return message;
}

std::shared_ptr<Message> deserializeToMessage(std::shared_ptr<NetworkMessage> message){

    switch (message->type) {
        case MessageType::NOTIFY_CLUSTER:
            return NodeInfoMessageFromRawBytes(message->data, message->length);
        default:
            throw std::runtime_error("Unknown message type");
    }

}