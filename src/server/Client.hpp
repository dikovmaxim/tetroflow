#pragma once

#include "json_fwd.hpp"

#include <memory>
#include <string>
#include <thread>

#include "../datatypes/Datatype.hpp"


#define READ_BUFFER_SIZE 2048

class Client{
public:
    Client(int socket);
    ~Client();
    void handle();
    void send(const nlohmann::json& json);
    void closeSocket();
private:
    bool hadling = true;
    int socket;
    std::thread handler;
};