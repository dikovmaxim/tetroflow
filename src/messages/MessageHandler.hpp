#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Message.hpp"
#include "MessageUtils.hpp"
#include "../server/json_fwd.hpp"
#include "../datatypes/Datatype.hpp"
#include "../gossip/Node.hpp"

bool messageInList(std::shared_ptr<Message> message);
void addMessageToExchangeQueue(std::shared_ptr<Message> message);

void startMessageExchangeQueue();
void stopMessageExchangeQueue();