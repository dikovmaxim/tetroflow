#pragma once
#include "json_fwd.hpp"

void startServer(std::string socketPath);
void writeError(int clientFd, const std::string& message);
void writeResponse(int clientFd, const nlohmann::json& response);