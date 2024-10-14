#pragma once

#include <string>

//types of messages sent and received using the gossip protocol

enum MessageType {
    MESSAGE_JOIN,
    MESSAGE_LEAVE,
    MESSAGE_HEARTBEAT,
    MESSAGE_REPLICATE,

    MESSAGE_UNKNOWN
};

std::string MessageTypeToString(MessageType type);
MessageType MessageTypeFromString(const std::string& type);