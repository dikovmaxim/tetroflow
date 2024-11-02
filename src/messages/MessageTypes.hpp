#pragma once

#include <string>

//types of messages sent and received using the gossip protocol

enum MessageType {
    NOTIFY_CLUSTER = 0,
};

std::string MessageTypeToString(MessageType type);
MessageType MessageTypeFromString(const std::string& type);