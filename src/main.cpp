
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cstdint>
#include <unordered_set>

#include "server/Server.hpp"

int main(int argc, char** argv) {

    std::string socketPath = "/tmp/test_socket";
    startServer(socketPath);


    return 0;
}