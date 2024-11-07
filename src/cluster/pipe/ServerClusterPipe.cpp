#include "ServerClusterPipe.hpp"
#include "../../log/Logger.hpp"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <thread>

ServerClusterPipe::ServerClusterPipe(int fdsock) {
    this->fdsock = fdsock;
    this->state = ClusterPipeState::CONNECTED;
    this->running = true;
    this->workerThread = std::thread(&ServerClusterPipe::HandleMessagesWorker, this);
}

void ServerClusterPipe::Connect() {
    throw std::runtime_error("Cannot connect a server pipe, only accept connections as a server");
}

void ServerClusterPipe::closePipe() {
    this->running = false;
    log(LOG_INFO, "Closing server pipe");
    if (std::this_thread::get_id() != this->workerThread.get_id() && this->workerThread.joinable()) {
        this->workerThread.join();
    }
    close(this->fdsock);
    log(LOG_INFO, "Server pipe closed");
    this->setState(ClusterPipeState::DISCONNECTED);
}

void ServerClusterPipe::sendBytes(std::vector<std::byte> message) {
    if (send(this->fdsock, message.data(), message.size(), 0) < 0) {
        throw std::runtime_error("Error sending message");
    }
}

void ServerClusterPipe::HandleMessagesWorker() {
    this->setState(ClusterPipeState::CONNECTED);
    while (this->running) {
        std::shared_ptr<std::vector<std::byte>> message = std::make_shared<std::vector<std::byte>>();
        char buffer[4096] = {0};
        int valread = read(this->fdsock, buffer, 1024);

        if (valread == 0) {
            log(LOG_INFO, "Client disconnected");
            this->closePipe();
            return;
        }

        if (valread < 0) {
            throw std::runtime_error("Error reading from socket");
        }

        message->resize(valread);
        for (int i = 0; i < valread; i++) {
            message->at(i) = std::byte(buffer[i]);
        }
        if(this->onMessage) {
            this->onMessage(message);
        }
    }
}