#pragma once

#include "./pipe/ClusterPipe.hpp"
#include "../messages/Message.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

void AddMessageToPool(std::shared_ptr<Message> message);
bool IsMessageInPool(std::shared_ptr<Message> message);
