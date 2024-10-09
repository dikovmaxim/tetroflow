#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>

#include "../Global.hpp"
#include "../datatypes/Datatype.hpp"
#include "json_fwd.hpp"

#include "Command.hpp"
#include "Operations.hpp"
#include "Cast.hpp"

#include "../datatypes/Datatype.hpp"
#include "../datatypes/Error.hpp"
#include "../datatypes/Set.hpp"
#include "../datatypes/List.hpp"
#include "../datatypes/Integer.hpp"
#include "../datatypes/Float.hpp"
#include "../datatypes/String.hpp"
#include "../datatypes/Boolean.hpp"

// Refactor Command class to handle arguments as a single object with key-value pairs


Command jsonToCommand(const std::string json) {
    // Parse the JSON string, if not possible, throw an exception
    nlohmann::json j;
    try {
        j = nlohmann::json::parse(json);
    } catch (nlohmann::json::parse_error& e) {
        throw std::invalid_argument("Invalid JSON");
    }

    // Create a new Command object
    Command command;

    // Get the command type from the JSON object
    std::string type = j["command"];

    command.json = j;

    // Convert the command type to a CommandType enum
    command.type = stringToCommandType(type);

    // Get the arguments from the JSON object as key-value pairs
    if (j.contains("arguments") && j["arguments"].is_object()) {
        for (auto& param : j["arguments"].items()) {
            command.args[param.key()] = param.value();  // Store each key-value pair in args map
        }
    }

    return command;
}

void printCommand(const Command& command) {
    for (const auto& [key, value] : command.args) {
        std::cout << key << ": " << value << std::endl;
    }
}

std::shared_ptr<DataType> JsonToDataType(nlohmann::json j) {
    if (j.is_boolean()) {
        return createBoolean(j.get<bool>());
    }
    else if (j.is_string()) {
        return createString(j.get<std::string>());
    } 
    else if (j.is_number_float()) {
        return createFloat(j.get<float>());
    } 
    else if (j.is_number_integer()) {
        return createInteger(j.get<int>());
    }
    else if (j.is_array()) {
        // Always treat arrays as List, whether empty or not
        std::list<std::shared_ptr<DataType>> listData;
        for (auto& item : j) {
            listData.push_back(JsonToDataType(item));
        }
        return createList(listData);
    }

    throw std::invalid_argument("Unsupported JSON structure");
}

std::shared_ptr<DataType> executeCommand(Command command, std::shared_ptr<Table> table) {
    // Get the command type
    CommandType type = command.type;

    // Get the JSON object
    nlohmann::json j = command.json;

    // Execute the command based on the type
    switch (type) {
        case CommandType::GET: {
            int keyInt = command.args.at("key");
            return operations::get(table, keyInt);
        }
        case CommandType::SET: {
            int keyInt = command.args.at("key");
            return operations::set(table, keyInt, JsonToDataType(command.args.at("value")));
        }
        case CommandType::DEL: {
            int keyInt = command.args.at("key");
            return operations::del(table, keyInt);
        }

        case CommandType::EXISTS: {
            int keyInt = command.args.at("key");
            return operations::exists(table, keyInt);
        }

        case CommandType::KEYS: {
            return operations::keys(table);
        }

        case CommandType::TYPE: {
            int keyInt = command.args.at("key");
            return operations::type(table, keyInt);
        }

        case CommandType::APPEND: {
            int keyInt = command.args.at("key");
            return operations::string::append(table, keyInt, command.args.at("value"));
        }

        case CommandType::STRLEN: {
            int keyInt = command.args.at("key");
            return operations::string::strlen(table, keyInt);
        }

        case CommandType::GETRANGE: {
            int keyInt = command.args.at("key");
            int start = command.args.at("start");
            int end = command.args.at("end");
            return operations::string::getrange(table, keyInt, start, end);
        }

        case CommandType::LPUSH: {
            int keyInt = command.args.at("key");
            return operations::list::lpush(table, keyInt, JsonToDataType(command.args.at("value")));
        }

        case CommandType::RPUSH: {
            int keyInt = command.args.at("key");
            return operations::list::rpush(table, keyInt, JsonToDataType(command.args.at("value")));
        }

        case CommandType::LPOP: {
            int keyInt = command.args.at("key");
            return operations::list::lpop(table, keyInt);
        }

        case CommandType::RPOP: {
            int keyInt = command.args.at("key");
            return operations::list::rpop(table, keyInt);
        }

        case CommandType::LLEN: {
            int keyInt = command.args.at("key");
            return operations::list::llen(table, keyInt);
        }

        case CommandType::LSET: {
            int keyInt = command.args.at("key");
            int index = command.args.at("index");
            return operations::list::lset(table, keyInt, index, JsonToDataType(command.args.at("value")));
        }
        
        case CommandType::LREM: {
            int keyInt = command.args.at("key");
            int count = command.args.at("count");
            return operations::list::lrem(table, keyInt, count, JsonToDataType(command.args.at("value")));
        }

        case CommandType::CAST: {
            int keyInt = command.args.at("key");
            std::string type = command.args.at("type");
            DataTypeType dataType = stringToDataTypeType(type);
            if(dataType == DataTypeType::UNDEFINED) {
                throw std::invalid_argument("Invalid data type");
            }
            std::shared_ptr<DataType> value = table->get(keyInt);
            std::shared_ptr<DataType> result = nullptr;
            try{
                result = Cast(value, dataType);
                table->set(keyInt, result);
            } catch (const std::exception& e) {
                throw std::invalid_argument(e.what());
            }
            return result;
        }

        case CommandType::SADD: {
            int keyInt = command.args.at("key");
            return operations::dataset::sadd(table, keyInt, JsonToDataType(command.args.at("value")));
        }

        case CommandType::SREM: {
            int keyInt = command.args.at("key");
            return operations::dataset::srem(table, keyInt, JsonToDataType(command.args.at("value")));
        }

        case CommandType::SCARD: {
            int keyInt = command.args.at("key");
            return operations::dataset::scard(table, keyInt);
        }

        case CommandType::ZADD: {
            int keyInt;
            float score;
            try {
                keyInt = command.args.at("key");
                score = command.args.at("score");
                return operations::sortedset::zadd(table, keyInt, JsonToDataType(command.args.at("value")), score);
            } catch (const std::exception& e) {
                return make_error("Invalid key, value or score");
            }
        }

        case CommandType::ZREM: {
            int keyInt = command.args.at("key");
            return operations::sortedset::zrem(table, keyInt, JsonToDataType(command.args.at("value")));
        }

        case CommandType::ZRANGE: {
            int keyInt = command.args.at("key");
            int start = command.args.at("start");
            int end = command.args.at("end");
            return operations::sortedset::zrange(table, keyInt, start, end);
        }

        case CommandType::ZREVRANGE: {
            int keyInt = command.args.at("key");
            int start = command.args.at("start");
            int end = command.args.at("end");
            return operations::sortedset::zrevrange(table, keyInt, start, end);
        }

        case CommandType::ZCARD: {
            int keyInt = command.args.at("key");
            return operations::sortedset::zcard(table, keyInt);
        }

        case CommandType::INC: {
            int keyInt = command.args.at("key");
            return operations::integer::inc(table, keyInt);
        }

        case CommandType::DEC: {
            int keyInt = command.args.at("key");
            return operations::integer::dec(table, keyInt);
        }

        case CommandType::INCBY: {
            int keyInt = command.args.at("key");
            int increment = command.args.at("increment");
            return operations::integer::incby(table, keyInt, increment);
        }

        case CommandType::DECBY: {
            int keyInt = command.args.at("key");
            int decrement = command.args.at("decrement");
            return operations::integer::decby(table, keyInt, decrement);
        }

        case CommandType::FLIP: {
            int keyInt = command.args.at("key");
            return operations::boolean::flip(table, keyInt);
        }



    }

    return std::make_shared<Error>("Invalid command");
}
