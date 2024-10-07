
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

    std::shared_ptr<Table> coreTable = std::make_shared<Table>(100);

    Command command = jsonToCommand(R"({
        "command": "SET",
        "arguments": {
            "key": 1,
            "value": 7
        }
    })");
    std::shared_ptr<DataType> result = executeCommand(command, coreTable);
    std::cout << "SET Command Result: " << result->to_string() << std::endl;

    command = jsonToCommand(R"({
        "command": "INC",
        "arguments": {
            "key": 1
        }
    })");
    result = executeCommand(command, coreTable);
    std::cout << "INC Command Result: " << result->to_string() << std::endl;


    //print the table
    coreTable->printBeautify();

    return 0;
}