#pragma once
#include <string>
void startServer(std::string socketPath);
void removeClientBySocket(int socket);
void stopServer();