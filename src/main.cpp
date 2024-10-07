
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <string>
#include <list>
#include <tuple>

#include "Global.hpp"
#include "log/Logger.hpp"
#include "table/Table.hpp"

#include "server/Command.hpp"


int main(int argc, char** argv) {

    initStorage();

    Command command = jsonToCommand("{\"command\": \"SET\",\"key\": 1,\"arguments\": [\"value1\", \"value2\"]}");
    std::shared_ptr<DataType> test = executeCommand(command, coreTable);

    command = jsonToCommand("{\"command\": \"SET\",\"key\": 3,\"arguments\": [\"fff\", \"ddd\", 0.001, 3]}");
    test = executeCommand(command, coreTable);

    command = jsonToCommand("{\"command\": \"SET\",\"key\": 7,\"arguments\": \"test\"}");
    test = executeCommand(command, coreTable);

    coreTable->print();

    //delete index 3
    command = jsonToCommand("{\"command\": \"DEL\",\"key\": 3}");
    test = executeCommand(command, coreTable);

    coreTable->print();

    //get index 1
    command = jsonToCommand("{\"command\": \"GET\",\"key\": 1}");
    test = executeCommand(command, coreTable);

    //exists index 1
    command = jsonToCommand("{\"command\": \"EXISTS\",\"key\": 1}");
    test = executeCommand(command, coreTable);

    std::cout << test->to_string() << std::endl;

    //get keys
    command = jsonToCommand("{\"command\": \"KEYS\"}");
    test = executeCommand(command, coreTable);

    std::cout << test->to_string() << std::endl;

    //get type
    command = jsonToCommand("{\"command\": \"TYPE\",\"key\": 1}");
    test = executeCommand(command, coreTable);

    std::cout << test->to_string() << std::endl;

    return 0;
}