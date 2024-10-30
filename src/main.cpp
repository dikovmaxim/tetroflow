
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <string>
#include <list>
#include <tuple>
#include <thread>
#include <string>

#include <stdlib.h>
#include <signal.h>

#include "Global.hpp"
#include "log/Logger.hpp"
#include "table/Table.hpp"

#include "datatypes/Datatype.hpp"
#include "transactions/Transaction.hpp"
#include "transactions/TransactionManager.hpp"

#include "server/Command.hpp"
#include "server/Operations.hpp"
#include "server/Server.hpp"
#include "server/json_fwd.hpp"

#include "gossip/nodes/ClientNode.hpp"
#include "gossip/GossipServer.hpp"
#include "messages/MessageHandler.hpp"

#include "gossip/GossipManager.hpp"

#include "config/ConfigParser.hpp"

#include "signals/Signals.hpp"

int main(int argc, char** argv) {

    std::string configFile = "tetroflow.cfg";

    //if 1 argument is passed, it is the config file
    if (argc == 2) {
        configFile = std::string(argv[1]);
    }

    std::string replicateListenInterface = "0.0.0.0";
    int replicateListenPort = 2732;

    std::string socketPath = "/tmp/tetroflow";

    std::vector<std::string> replicateConnect = {};

    int tableReserveSize = 1000;

    log(LOG_INFO, "Reading configuration file: " + configFile);

    try {
        ConfigParser config(configFile);

        socketPath = config.getString("socket");

        std::string replicateListen = config.getString("replicateListen");
        replicateListenInterface = replicateListen.substr(0, replicateListen.find(':'));
        replicateListenPort = std::stoi(replicateListen.substr(replicateListen.find(':') + 1));

        replicateConnect = config.getList("replicateConnect");

        tableReserveSize = config.getInt("tableReserveSize");

        for (const std::string& connect : replicateConnect) {
            std::string ip = connect.substr(0, connect.find(':'));
            int port = std::stoi(connect.substr(connect.find(':') + 1));
            std::shared_ptr<Node> node = connectToNode(ip, port);
            addNode(node);
        }

    } catch (const std::exception& ex) {
        log(LOG_ERROR, "Error parsing configuration file: " + std::string(ex.what()));
        return -1;
    }

    log(LOG_INFO, "Starting TetroFlow");

    signal(SIGSEGV, sig_handler);
    signal (SIGABRT, sig_handler);
    signal (SIGFPE, sig_handler);
    signal (SIGILL, sig_handler);
    signal (SIGINT, sig_handler);
    signal (SIGSEGV, sig_handler);
    signal (SIGTERM, sig_handler);

    log(LOG_INFO, "Started signal handling");

    initStorage(tableReserveSize);

    startTransactionHandling();

    startMessageExchangeQueue();
    
    startGossipServer(replicateListenInterface, replicateListenPort);

    startServer(socketPath);

    return 0;
}