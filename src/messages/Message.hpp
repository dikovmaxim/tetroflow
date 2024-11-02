#pragma once

#include "MessageTypes.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdint>
#include <vector>


class Message {

public:
    virtual MessageType getType() = 0;
    virtual std::vector<std::byte> serialize() = 0;
    virtual void deserialize(const std::vector<std::byte>& bytes) = 0;
};