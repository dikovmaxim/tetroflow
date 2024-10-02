#ifndef COMMAND_EXECUTOR_HPP
#define COMMAND_EXECUTOR_HPP

#include "json_fwd.hpp"
#include "../Global.hpp"
#include "Server.hpp"
#include "json_fwd.hpp"

void ExecuteCommand(int clientFd, const std::string& command, const nlohmann::json& parameters);

#endif // COMMAND_EXECUTOR_HPP