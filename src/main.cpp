
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
#include "messages/messagetypes/MessageJoin.hpp"
#include "messages/MessageHandler.hpp"

#include "gossip/GossipManager.hpp"

int main(int argc, char** argv) {

    initStorage();
    startTransactionHandling();


    std::shared_ptr<Node> node = connectToNode("127.0.0.1", 4444);

    std::shared_ptr<Message> message = std::make_shared<MessageJoin>("1234");
    node->addMessageToQueue(message);

    startMessageExchangeQueue();
    
    startGossipServer();

    startServer();

    return 0;
}