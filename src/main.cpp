
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

#include "datatypes/Datatype.hpp"
#include "transactions/Transaction.hpp"
#include "transactions/TransactionManager.hpp"

#include "server/Command.hpp"

void testAllCommands() {
    // Test SET command (Create an integer)
    Command command = jsonToCommand(R"({
        "command": "SET",
        "arguments": {"key": 1, "value": 100}
    })");
    std::shared_ptr<DataType> result = executeCommand(command, coreTable);
    std::cout << "SET Command Result (Integer): " << result->to_string() << std::endl;

    // Test GET command
    command = jsonToCommand(R"({
        "command": "GET",
        "arguments": {"key": 1}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "GET Command Result (Integer): " << result->to_string() << std::endl;

    // Test DEL command
    command = jsonToCommand(R"({
        "command": "DEL",
        "arguments": {"key": 1}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "DEL Command Result: " << result->to_string() << std::endl;

    // Test EXISTS command
    command = jsonToCommand(R"({
        "command": "EXISTS",
        "arguments": {"key": 1}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "EXISTS Command Result: " << result->to_string() << std::endl;

    // Test TYPE command (Type of deleted key should return undefined)
    command = jsonToCommand(R"({
        "command": "TYPE",
        "arguments": {"key": 1}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "TYPE Command Result (Deleted key): " << result->to_string() << std::endl;

    // Test APPEND command (String)
    command = jsonToCommand(R"({
        "command": "SET",
        "arguments": {"key": 2, "value": "Hello"}
    })");
    executeCommand(command, coreTable);
    
    command = jsonToCommand(R"({
        "command": "APPEND",
        "arguments": {"key": 2, "value": " World"}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "APPEND Command Result: " << result->to_string() << std::endl;

    // Test STRLEN command
    command = jsonToCommand(R"({
        "command": "STRLEN",
        "arguments": {"key": 2}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "STRLEN Command Result: " << result->to_string() << std::endl;

    // Test GETRANGE command
    command = jsonToCommand(R"({
        "command": "GETRANGE",
        "arguments": {"key": 2, "start": 0, "end": 4}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "GETRANGE Command Result: " << result->to_string() << std::endl;

    // Test LPUSH and RPUSH commands (List)
    command = jsonToCommand(R"({
        "command": "SET",
        "arguments": {"key": 3, "value": []}
    })");
    executeCommand(command, coreTable);

    command = jsonToCommand(R"({
        "command": "LPUSH",
        "arguments": {"key": 3, "value": "first"}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "LPUSH Command Result: " << result->to_string() << std::endl;

    command = jsonToCommand(R"({
        "command": "RPUSH",
        "arguments": {"key": 3, "value": "second"}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "RPUSH Command Result: " << result->to_string() << std::endl;

    // Test LLEN command
    command = jsonToCommand(R"({
        "command": "LLEN",
        "arguments": {"key": 3}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "LLEN Command Result: " << result->to_string() << std::endl;

    // Test LPOP and RPOP commands
    command = jsonToCommand(R"({
        "command": "LPOP",
        "arguments": {"key": 3}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "LPOP Command Result: " << result->to_string() << std::endl;

    command = jsonToCommand(R"({
        "command": "RPOP",
        "arguments": {"key": 3}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "RPOP Command Result: " << result->to_string() << std::endl;

    // Test LSET command
    command = jsonToCommand(R"({
        "command": "RPUSH",
        "arguments": {"key": 3, "value": "new_element"}
    })");
    executeCommand(command, coreTable);

    command = jsonToCommand(R"({
        "command": "LSET",
        "arguments": {"key": 3, "index": 0, "value": "updated_element"}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "LSET Command Result: " << result->to_string() << std::endl;

    // Test INC and DEC commands (Integer)
    command = jsonToCommand(R"({
        "command": "SET",
        "arguments": {"key": 4, "value": 10}
    })");
    executeCommand(command, coreTable);

    command = jsonToCommand(R"({
        "command": "INC",
        "arguments": {"key": 4}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "INC Command Result: " << result->to_string() << std::endl;

    command = jsonToCommand(R"({
        "command": "DEC",
        "arguments": {"key": 4}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "DEC Command Result: " << result->to_string() << std::endl;

    // Test FLIP command (Boolean)
    command = jsonToCommand(R"({
        "command": "SET",
        "arguments": {"key": 5, "value": true}
    })");
    executeCommand(command, coreTable);

    command = jsonToCommand(R"({
        "command": "FLIP",
        "arguments": {"key": 5}
    })");
    result = executeCommand(command, coreTable);
    std::cout << "FLIP Command Result: " << result->to_string() << std::endl;
}

void transactionCallback(const std::shared_ptr<DataType>& result, int socket) {
    std::cout << "Transaction callback: " << result->to_string() << std::endl;
}

int main(int argc, char** argv) {

    initStorage();

    Command command = jsonToCommand(R"({
        "command": "SET",
        "arguments": {"key": 1, "value": 100}
    })");

    Transaction transaction = makeSingleCommandTransaction(command, coreTable, transactionCallback, 0);
    addTransaction(std::make_shared<Transaction>(transaction));

    startTransactionHandling();

    return 0;
}