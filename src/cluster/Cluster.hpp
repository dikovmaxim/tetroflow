#pragma once

#include "./pipe/ClusterPipe.hpp"
#include "../messages/Message.hpp"
#include "./ClusterNode.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

extern std::vector<std::shared_ptr<ClusterNode>> nodes;
extern std::vector<std::shared_ptr<Message>> messagePool;

void AddMessageToPool(std::shared_ptr<Message> message);
bool IsMessageInPool(std::shared_ptr<Message> message);

bool NodeExists(std::shared_ptr<ClusterNode> node);
void AddNode(std::shared_ptr<ClusterNode> node);
void RemoveNode(std::shared_ptr<ClusterNode> node);



void initClusterServer();