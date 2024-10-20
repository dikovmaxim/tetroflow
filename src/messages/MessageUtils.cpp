#include "MessageUtils.hpp"
#include <random>
#include <string>
#include <iostream>
#include <chrono>

#define node_id 777

//generate a 32 bit unique id
int generate_unique_id() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2147483647);
    return dis(gen);
}

int generate_unique_timestamp() {
    //return a 32 bit integer to represented in json
    //in form yyyymmddhhmmssmsms
    std::string timestamp = "";
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::gmtime(&time);
    timestamp += std::to_string(tm->tm_mon);
    timestamp += std::to_string(tm->tm_mday);
    timestamp += std::to_string(tm->tm_hour);
    timestamp += std::to_string(tm->tm_min);
    timestamp += std::to_string(tm->tm_sec);

    return std::stoi(timestamp);
}