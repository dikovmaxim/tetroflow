#include "Cluster.hpp"
#include "ClusterNode.hpp"
#include "ClusterServer.hpp"
#include "../log/Logger.hpp"

#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>


std::vector<std::shared_ptr<ClusterNode>> nodes;
std::vector<std::shared_ptr<Message>> messagePool;

std::shared_ptr<ClusterServer> server;

void initClusterServer() {

    log(LOG_INFO, "Initializing cluster server");

    server = std::make_shared<ClusterServer>(8080, "0.0.0.0");
    server->start();
}