#pragma once

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

#include "Node.hpp"

void addNode(std::shared_ptr<Node> node);
void removeNode(Node& node);
void testGossip();

void broadcastMessage(std::shared_ptr<Message> message);

std::shared_ptr<Node> connectToNode(std::string ip, int port);

std::vector<std::shared_ptr<Node>> getNodes();