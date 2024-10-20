#include "GossipManager.hpp"
#include "Node.hpp"
#include "nodes/ClientNode.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>


std::vector<std::shared_ptr<Node>> nodes;

void addNode(std::shared_ptr<Node> node) {
    nodes.push_back(node);
}

//node contains a == operator, its NOT guaranteed that the node has an address
void removeNode(Node& node) {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        if ((*it)->compare(node)) {
            nodes.erase(it);
            return;
        }
    }
}

void gossip() {
    while (true) {
        for (auto& node : nodes) {
            std::cout << "Gossiping with " << node->toString() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void testGossip() {

}

std::vector<std::shared_ptr<Node>> getNodes() {
    return nodes;
}

void broadcastMessage(std::shared_ptr<Message> message) {
    for (auto& node : nodes) {
        std::cout << "Sending message to " << node->toString() << std::endl;
        node->addMessageToQueue(message);
    }
}

std::shared_ptr<Node> connectToNode(std::string ip, int port) {
    // Create a new client node
    std::shared_ptr<Node> node = std::make_shared<ClientNode>(ip, port);
    addNode(node);
    node->start();
    return node;
}