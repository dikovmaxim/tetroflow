#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <memory>

#include "Server.hpp"
#include "../log/Logger.hpp"
#include "Client.hpp"
#include "Operations.hpp"
#include "../transactions/Transaction.hpp"
#include "../transactions/TransactionManager.hpp"


std::vector<std::shared_ptr<Client>> clients;

void printClients() {
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        log(LOG_INFO, "Number of clients: " + std::to_string(clients.size()));
    }
}

void removeClientBySocket(int socket) {
    for (auto it = clients.begin(); it != clients.end(); it++) {
        if ((*it)->getSocket() == socket) {
            clients.erase(it);
            return;
        }
    }
}

//the server socket is reusable .sock file
void startServer() {

    //std::thread tt = std::thread(printClients);
    //tt.detach();

    int serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        log(LOG_ERROR, "Failed to create server socket");
        return;
    }

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        log(LOG_ERROR, "Failed to set socket options");
        close(serverSocket);
        return;
    }

    sockaddr_un address;
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, SOCKET_PATH);
    unlink(SOCKET_PATH);

    if (bind(serverSocket, (sockaddr*)&address, sizeof(address)) == -1) {
        log(LOG_ERROR, "Failed to bind server socket");
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, 5) == -1) {
        log(LOG_ERROR, "Failed to listen on server socket");
        close(serverSocket);
        return;
    }

    log(LOG_INFO, "Server started");
    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            log(LOG_ERROR, "Failed to accept client connection");
            continue;
        }

        std::shared_ptr<Client> client = std::make_shared<Client>(clientSocket);
        clients.push_back(client);
    }
}