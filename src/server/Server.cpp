
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <thread>
//add inaddr and htons
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "Server.hpp"
#include "json_fwd.hpp"
#include "CommandExecutor.hpp"
#include "../log/Logger.hpp"

void writeError(int clientFd, const std::string& message) {
    std::string errorMessage = "{\"error\": \"" + message + "\"}";
    write(clientFd, errorMessage.c_str(), errorMessage.size());
    close(clientFd);
}

void writeResponse(int clientFd, const nlohmann::json& response) {
    std::string responseString = response.dump();
    write(clientFd, responseString.c_str(), responseString.size());
    close(clientFd);
}

void handleClient(int clientFd) {
    char buffer[1024];
    ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        log(LOG_ERROR, "Failed to read from socket");
        close(clientFd);
        return;
    }

    //parse the message as JSON
    nlohmann::json json;

    try {
        json = nlohmann::json::parse(std::string(buffer, bytesRead));
    } catch (const nlohmann::json::parse_error& e) {
        writeError(clientFd, "Failed to parse JSON");
        return;
    }

    std::cout << json.dump(4) << std::endl;

    // Check if the JSON object has a "command" field
    if (!json.contains("command")) {
        writeError(clientFd, "Missing 'command' field");
        return;
    }

    std::string command = json["command"];
    //also get an optional object parameters
    nlohmann::json parameters;
    if (json.contains("parameters")) {
        parameters = json["parameters"];
    }

    // Execute the command
    ExecuteCommand(clientFd, command, parameters);

    close(clientFd);
}

void startServer(std::string socketPath) {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        log(LOG_ERROR, "Failed to create socket");
        return;
    }

    // Make the socket reusable
    int reuse = 1;
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        log(LOG_ERROR, "Failed to set socket options");
        return;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        log(LOG_ERROR, "Failed to bind socket");
        return;
    }

    if (listen(serverFd, 5) == -1) {
        log(LOG_ERROR, "Failed to listen on socket");
        return;
    }

    log(LOG_INFO, "Server started on port 8080");

    while (true) {
        int clientFd = accept(serverFd, NULL, NULL);
        if (clientFd == -1) {
            std::cerr << "Failed to accept connection" << std::endl;
            return;
        }
        log(LOG_INFO, "Accepted connection");
        std::thread(handleClient, clientFd).detach();
    }

    // Close the server socket when done
    close(serverFd);
}
