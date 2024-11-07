#pragma once

#include "./pipe/ClusterPipe.hpp"
#include "../messages/Message.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdexcept>

class ClusterServer {
    private:
        bool running;
        std::thread workerThread;
        uint16_t port;
        std::string listenIP;
        void onClientConnected(int clientfd);
    public:
        ClusterServer(uint16_t port, std::string listenIP);
        ~ClusterServer();
        void start();
        void stop();
};