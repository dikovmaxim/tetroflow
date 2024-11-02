#include "MessageUtils.hpp"
#include <random>
#include <string>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <memory>

uint64_t generate_unique_id() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2147483647);
    return dis(gen);
}

int64_t generate_unique_timestamp() {
    int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return timestamp;
}
