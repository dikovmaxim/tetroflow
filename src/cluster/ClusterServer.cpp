#include "ClusterServer.hpp"
#include "pipe/ClusterPipe.hpp"
#include "ClusterNode.hpp"
#include "pipe/ServerClusterPipe.hpp"
#include "Cluster.hpp"
#include "../log/Logger.hpp"

ClusterServer::ClusterServer(uint16_t port, std::string listenIP) {
    this->port = port;
    this->listenIP = listenIP;
    this->running = false;

    log(LOG_INFO, "Cluster server created");
}

ClusterServer::~ClusterServer() {
    this->stop();
}

void ClusterServer::onClientConnected(int clientfd) {
    //create a new pipe for the client
    std::shared_ptr<ClusterNode> node = std::make_shared<ClusterNode>();
    std::shared_ptr<ServerClusterPipe> pipe = std::make_shared<ServerClusterPipe>(clientfd);

    node->InitializePipe(pipe);

    //add the node to the cluster
    AddNode(node);
}

void ClusterServer::start() {

    log(LOG_INFO, "Starting cluster server");

    if (this->running) {
        return;
    }

    this->running = true;

    //create a socket
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    //bind the socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(this->listenIP.c_str());
    address.sin_port = htons(this->port);
    if (bind(serverfd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        throw std::runtime_error("Failed to bind socket");
    }

    //listen for incoming connections
    if (listen(serverfd, 10) == -1) {
        throw std::runtime_error("Failed to listen on socket");
    }

    log(LOG_INFO, "Cluster server listening on " + this->listenIP + ":" + std::to_string(this->port));

    //accept incoming connections
    while (this->running) {
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientfd = accept(serverfd, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientfd == -1) {
            throw std::runtime_error("Failed to accept incoming connection");
        }

        //handle the new client
        this->onClientConnected(clientfd);
    }

    //close the server socket
    close(serverfd);
}

void ClusterServer::stop() {
    this->running = false;
    if (this->workerThread.joinable()) {
        this->workerThread.join();
    }
}

