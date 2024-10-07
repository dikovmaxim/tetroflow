
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

   //create a list on 1st key
    Command command = jsonToCommand(R"({
        "command": "SET",
        "arguments": {
            "key": 1,
            "value": []
        }
    })");
    std::shared_ptr<DataType> result = executeCommand(command, coreTable);
    std::cout << "SET Command Result: " << result->to_string() << std::endl;

    // Test LPUSH Command (Left Push)
    command = jsonToCommand(R"({
        "command": "LPUSH", 
        "arguments": {"key": 1, "value": "first"}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "LPUSH Command Result: " << result->to_string() << std::endl;

    command = jsonToCommand(R"({
        "command": "LPUSH", 
        "arguments": {"key": 1, "value": "second"}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "LPUSH Command Result: " << result->to_string() << std::endl;

    // Test LLEN Command (List Length)
    command = jsonToCommand(R"({
        "command": "LLEN", 
        "arguments": {"key": 1}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "LLEN Command Result: " << result->to_string() << std::endl;

    // Test RPUSH Command (Right Push)
    command = jsonToCommand(R"({
        "command": "RPUSH", 
        "arguments": {"key": 1, "value": [1,2,3,4,5,6,7,0.0777]}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "RPUSH Command Result: " << result->to_string() << std::endl;

    // Test LLEN Command (List Length) after pops
    command = jsonToCommand(R"({
        "command": "LLEN", 
        "arguments": {"key": 1}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "LLEN Command Result after pops: " << result->to_string() << std::endl;

    //print the table
    coreTable->printBeautify();

    return 0;
}