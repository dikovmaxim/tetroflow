
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

#include "config/ConfigParser.hpp"

#include "cluster/ClusterNode.hpp"
#include "messages/Message.hpp"
#include "messages/MessageTypes.hpp"
#include "messages/messagetypes/NodeInfoMessage.hpp"
#include "messages/NetworkMessage.hpp"
#include "messages/MessageUtils.hpp"


#include "signals/Signals.hpp"

int r_main(int argc, char** argv) {

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

    startServer(socketPath);

    return 0;
}


int main(int argc, char** argv) {
    


    std::vector<std::byte> readBytes;
    FILE* file = fopen("test.bin", "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    readBytes.resize(fileSize);
    fread(readBytes.data(), sizeof(std::byte), fileSize, file);
    fclose(file);


    std::shared_ptr<NetworkMessage> message = networkMessageFromBytes(readBytes);

    std::shared_ptr<NodeInfoMessage> nodeInfoMessage = std::dynamic_pointer_cast<NodeInfoMessage>(deserializeToMessage(message));

    //dynamic cast to NodeInfoMessage
    std::cout << nodeInfoMessage->toString() << std::endl;


    return 0;
}