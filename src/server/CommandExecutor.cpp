
#include <iostream>
#include <string>

#include "CommandExecutor.hpp"
#include "../Global.hpp"

void ExecuteCommand(int clientFd, const std::string& command, const nlohmann::json& parameters) {

    if (command == "ping") {
        writeResponse(clientFd, {{"response", "pong"}});
        return;
    }

    writeError(clientFd, "Unknown command");

}
