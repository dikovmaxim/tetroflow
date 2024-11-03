#include "ClientClusterPipe.hpp"

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

ClientClusterPipe::ClientClusterPipe(std::string ip, int port) : ip(ip), port(port) {
    fdsock = socket(AF_INET, SOCK_STREAM, 0);
    if (fdsock == -1) {
        throw std::runtime_error("Error creating socket");
    }
    Connect();
}

void ClientClusterPipe::Connect() {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (connect(fdsock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        throw std::runtime_error("Error connecting to server");
    }
    running = true;
    workerThread = std::thread(&ClientClusterPipe::HandleMessagesWorker, this);
}

void ClientClusterPipe::sendBytes(std::vector<std::byte> message) {
    if (send(fdsock, message.data(), message.size(), 0) < 0) {
        throw std::runtime_error("Error sending message");
    }
}

void ClientClusterPipe::closePipe() {
    running = false;
    workerThread.join();
    close(fdsock);
}

void ClientClusterPipe::HandleMessagesWorker() {
    this->setState(ClusterPipeState::CONNECTED);
    while (running) {
        std::shared_ptr<std::vector<std::byte>> message = std::make_shared<std::vector<std::byte>>();
        char buffer[4096] = {0};
        int valread = read(fdsock, buffer, 1024);
        if (valread <= 0) {
            break;
        }
        message->resize(valread);
        for (int i = 0; i < valread; i++) {
            message->at(i) = std::byte(buffer[i]);
        }
        if(this->onMessage) {
            this->onMessage(message);
        }
    }
    this->setState(ClusterPipeState::DISCONNECTED);
    this->closePipe();
}