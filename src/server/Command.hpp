#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

#include "../datatypes/Datatype.hpp"
#include "../table/Table.hpp"
#include "json_fwd.hpp"

enum CommandType {
    GET,
    SET,
    DEL,
    EXISTS,
    KEYS,
    TYPE,
    EXPIRE,
    INVALID
};


typedef struct Command {
    CommandType type;
    std::vector<nlohmann::json> args;
    nlohmann::json json;
} Command;

std::shared_ptr<DataType> executeCommand(Command command, std::shared_ptr<Table> table);
Command jsonToCommand(std::string json);

inline CommandType stringToCommandType(std::string type){
    if (type == "GET") return CommandType::GET;
    if (type == "SET") return CommandType::SET;
    if (type == "DEL") return CommandType::DEL;
    if (type == "EXISTS") return CommandType::EXISTS;
    if (type == "KEYS") return CommandType::KEYS;
    if (type == "TYPE") return CommandType::TYPE;
    if (type == "EXPIRE") return CommandType::EXPIRE;

    return CommandType::INVALID;
}

void printCommand(Command command);