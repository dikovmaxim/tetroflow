#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "MessageTypes.hpp"
#include "Message.hpp"

typedef struct NetworkMessage {
    uint64_t id;
    uint64_t timestamp;
    uint64_t sender;
    uint64_t length;
    uint16_t checksum;
    uint8_t type;
    char* data;
} NetworkMessage;

std::shared_ptr<NetworkMessage> createNetworkMessage(uint64_t id, uint64_t timestamp, uint64_t sender, MessageType type, std::vector<std::byte> data);
std::shared_ptr<NetworkMessage> networkMessageFromBytes(std::vector<std::byte> bytes);

std::shared_ptr<Message> deserializeToMessage(std::shared_ptr<NetworkMessage> message);