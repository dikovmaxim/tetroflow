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
    APPEND,
    STRLEN,
    GETRANGE,
    LPUSH,
    RPUSH,
    LPOP,
    RPOP,
    LLEN,
    LSET,
    LINSERT,
    LREM,
    CAST,

    INVALID
};


struct Command {
    CommandType type;
    nlohmann::json json;
    std::unordered_map<std::string, nlohmann::json> args;  // Using map for key-value args
};

std::shared_ptr<DataType> executeCommand(Command command, std::shared_ptr<Table> table);
Command jsonToCommand(const std::string json);

inline CommandType stringToCommandType(std::string type){
    if (type == "GET") return CommandType::GET;
    if (type == "SET") return CommandType::SET;
    if (type == "DEL") return CommandType::DEL;
    if (type == "EXISTS") return CommandType::EXISTS;
    if (type == "KEYS") return CommandType::KEYS;
    if (type == "TYPE") return CommandType::TYPE;
    if (type == "APPEND") return CommandType::APPEND;
    if (type == "STRLEN") return CommandType::STRLEN;
    if (type == "GETRANGE") return CommandType::GETRANGE;
    if (type == "LPUSH") return CommandType::LPUSH;
    if (type == "RPUSH") return CommandType::RPUSH;
    if (type == "LPOP") return CommandType::LPOP;
    if (type == "RPOP") return CommandType::RPOP;
    if (type == "LLEN") return CommandType::LLEN;
    if (type == "LSET") return CommandType::LSET;
    if (type == "LINSERT") return CommandType::LINSERT;
    if (type == "LREM") return CommandType::LREM;
    if (type == "CAST") return CommandType::CAST;
    

    return CommandType::INVALID;
}

void printCommand(Command command);