#include "Client.hpp"

#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <memory>
#include <cstring>
#include <string>
#include <functional>


#include "json_fwd.hpp"

#include "../log/Logger.hpp"
#include "Command.hpp"
#include "Operations.hpp"
#include "../transactions/Transaction.hpp"
#include "../transactions/TransactionManager.hpp"


Client::Client(int socket) {
    this->socket = socket;
    this->handler = std::thread(&Client::handle, this);
    this->handler.detach();
}

Client::~Client() {
    closeSocket();
}

void transactionCallback(const std::shared_ptr<DataType> data, Client& client) {
    std::cout << "Transaction callback for client" << std::endl;
}


void Client::handle() {
    while (true) {
        char buffer[READ_BUFFER_SIZE];
        ssize_t bytesRead = read(socket, buffer, READ_BUFFER_SIZE);

        if (bytesRead == -1) {
            log(LOG_ERROR, "Failed to read from socket");
            break;
        }

        if (bytesRead == 0) {
            log(LOG_INFO, "Client disconnected");
            break;
        }

        Command command;

        try {
            command = jsonToCommand(std::string(buffer, bytesRead));
        } catch (const nlohmann::json::parse_error& e) {
            log(LOG_ERROR, std::string("Failed to parse JSON: ") + e.what());
            continue;
        }

        
        //print a command json
        std::cout << command.json.dump() << std::endl;

    }

    closeSocket();
}

void Client::send(const nlohmann::json& json) {
    std::string jsonString = json.dump();
    ssize_t bytesSent = write(socket, jsonString.c_str(), jsonString.size());
}

void Client::closeSocket() {
    // Wait for the thread to stop, detach if necessary
    if (handler.joinable()) {
        handler.join();  // Wait for the thread to stop
    }

    // Close the socket (if not already closed by the thread)
    close(socket);
}