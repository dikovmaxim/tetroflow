#include "MessageTypes.hpp"

std::string MessageTypeToString(MessageType type) {
    switch (type) {
        case MessageType::MESSAGE_JOIN:
            return "JOIN";
        case MessageType::MESSAGE_LEAVE:
            return "LEAVE";
        case MessageType::MESSAGE_HEARTBEAT:
            return "HEARTBEAT";
        case MessageType::MESSAGE_REPLICATE:
            return "REPLICATE";
        default:
            return "UNKNOWN";
    }
}

MessageType MessageTypeFromString(const std::string& type) {
    if (type == "JOIN") return MessageType::MESSAGE_JOIN;
    if (type == "LEAVE") return MessageType::MESSAGE_LEAVE;
    if (type == "HEARTBEAT") return MessageType::MESSAGE_HEARTBEAT;
    if (type == "REPLICATE") return MessageType::MESSAGE_REPLICATE;

    return MessageType::MESSAGE_UNKNOWN;
}