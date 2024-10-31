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
#include "../Global.hpp"
#include "Server.hpp"
#include "../datatypes/Datatype.hpp"



Client::Client(int socket) {
    this->socket = socket;
    this->handler = std::thread(&Client::handle, this);
    this->handler.detach();
}

Client::Client() {

}

Client::~Client() {
    closeSocket();
}

void transactionCallback(const std::shared_ptr<DataType> data, Client& client) {

    //parse data.t0_string() to json

    //if data is a null pointer, return an error
    if(data == nullptr) {
        return;
    }

    std::string dataString = data->to_string();
    nlohmann::json json = nlohmann::json::parse(dataString);

    //create a json object.
    nlohmann::json response = {
        {"response", json}
    };


    //send the json object to the client
    client.send(response);

}


void Client::handle() {
    while (hadling) {
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
            nlohmann::json json = nlohmann::json::parse(buffer);
            command = jsonToCommand(json.dump());
        } catch (const std::exception& e) {
            log(LOG_ERROR, std::string("Failed to parse JSON: ") + e.what());

            nlohmann::json response = {
                {"error", "Invalid JSON"}
            };

            send(response);

            continue;
        }

        //clean the buffer
        memset(buffer, 0, READ_BUFFER_SIZE);
        

        
        Transaction transaction = makeSingleCommandTransaction(
            command,
            coreTable,
            transactionCallback,
            *this
        );

        addTransaction(std::make_shared<Transaction>(transaction));

    }

    closeSocket();
}

void Client::send(const nlohmann::json& json) {
    std::string jsonString = json.dump();
    ssize_t bytesSent = write(socket, jsonString.c_str(), jsonString.size());
}

void Client::closeSocket() {
    // Wait for the thread to stop, detach if necessary

    hadling = false;

    if (handler.joinable()) {
        handler.join();  // Wait for the thread to stop
    }

    removeClientBySocket(socket);

    // Close the socket (if not already closed by the thread)
    close(socket);
}

int Client::getSocket() const {
    return socket;
}