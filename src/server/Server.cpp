
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <thread>

#include "Server.hpp"
#include "json_fwd.hpp"

void handleClient(int clientFd) {
    char buffer[1024];
    ssize_t bytesRead = read(clientFd, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        std::cerr << "Failed to read from client" << std::endl;
        close(clientFd);
        return;
    }

    std::cout << "Received message: " << std::string(buffer, bytesRead) << std::endl;

    //parse the message as JSON
    nlohmann::json json = nlohmann::json::parse(std::string(buffer, bytesRead));

    //print the JSON
    std::cout << json.dump(4) << std::endl;

    close(clientFd);
}

void startServer(std::string socketPath) {
    int serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverFd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    // Make the socket reusable
    int reuse = 1;
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        std::cerr << "Failed to set SO_REUSEADDR" << std::endl;
        return;
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path) - 1);
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';  // Null-terminate the path

    // Unlink the socket path to avoid "address already in use" error
    unlink(socketPath.c_str());

    if (bind(serverFd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        return;
    }

    if (listen(serverFd, 5) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return;
    }

    std::cout << "Server started" << std::endl;

    while (true) {
        int clientFd = accept(serverFd, NULL, NULL);
        if (clientFd == -1) {
            std::cerr << "Failed to accept connection" << std::endl;
            return;
        }
        std::thread(handleClient, clientFd).detach();

    }

    // Close the server socket when done
    close(serverFd);
}

