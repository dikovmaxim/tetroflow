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