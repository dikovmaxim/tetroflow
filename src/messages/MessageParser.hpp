#pragma once

#include <string>

#include "Message.hpp"
#include "../server/json_fwd.hpp"

std::shared_ptr<Message> parseMessage(nlohmann::json message);