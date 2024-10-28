#include "GossipServer.hpp"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "nodes/ClientNode.hpp"
#include "nodes/ServerNode.hpp"
#include "Node.hpp"
#include "GossipManager.hpp"
#include "../messages/Message.hpp"

#include "../log/Logger.hpp"

//this node is created when a new client connects to the server
std::thread gossipServerThread;
bool gossipRunning = true;
int gossip_server_socket;

void GossipServerHandler(int gossip_server_socket) {
    listen(gossip_server_socket, 3);
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    while (gossipRunning) {
        if ((new_socket = accept(gossip_server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            log(LOG_ERROR, "Accept failed on gossip server listening to socket " + std::to_string(gossip_server_socket));
            return;
        }
        log(LOG_INFO, "New gossip server connection as client");

        std::shared_ptr<ServerNode> serverNode = std::make_shared<ServerNode>(new_socket);
        serverNode->start();
        addNode(serverNode);
        
    }
}

void startGossipServer(std::string ip, int port) {
    
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    if ((gossip_server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        log(LOG_ERROR, "Gossip server socket creation failed");
        return;
    }
    if (setsockopt(gossip_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        log(LOG_ERROR, "Gossip server setsockopt failed");
        return;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    address.sin_port = htons(port);
    if (bind(gossip_server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        log(LOG_ERROR, "Gossip server bind failed");
        return;
    }
    log(LOG_INFO, "Gossip server listening on " + ip + ":" + std::to_string(port));
    gossipServerThread = std::thread(GossipServerHandler, gossip_server_socket);
}

void stopGossipServer() {
    gossipRunning = false;
    close(gossip_server_socket);
    if (gossipServerThread.joinable()) {
        // Get the native handle of the thread
        pthread_t nativeHandle = gossipServerThread.native_handle();

        pthread_cancel(nativeHandle);

        // Optionally, you can still join the thread to clean up resources
        gossipServerThread.join();
    }

    log(LOG_INFO, "Gossip server stopped");
}