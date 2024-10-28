
#include "Node.hpp"

#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "GossipManager.hpp"
#include "../messages/Message.hpp"
#include "../messages/MessageParser.hpp"
#include "../messages/MessageHandler.hpp"
#include "../log/Logger.hpp"

//implement a function from the Node class
void Node::handleIncomingMessage(nlohmann::json j) {
    std::shared_ptr<Message> message = parseMessage(j);

    std::cout << "Received message: " << message->ToJson().dump() << std::endl;

    ReceivedMessages.push_back(message);

    addMessageToExchangeQueue(message);
}