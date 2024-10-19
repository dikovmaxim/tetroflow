#include "GossipManager.hpp"
#include "Node.hpp"

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