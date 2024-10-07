#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "../Global.hpp"
#include "../datatypes/Datatype.hpp"
#include "json_fwd.hpp"

#include "Command.hpp"
#include "Operations.hpp"

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
    if (j.is_string()) {
        return createString(j.get<std::string>());
    } 
    else if (j.is_number_float()) {
        return createFloat(j.get<float>());
    } 
    else if (j.is_number_integer()) {
        return createInteger(j.get<int>());
    }
    else if (j.is_boolean()) {
        return createBoolean(j.get<bool>());
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

    std::cout << "Executing command: " << j << std::endl;

    // Execute the command based on the type
    switch (type) {
        case CommandType::GET: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
            } catch (std::out_of_range&) {
                return createString("Invalid key");
            }
            return operations::get(table, keyInt);
        }
        case CommandType::SET: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
                return operations::set(table, keyInt, JsonToDataType(command.args.at("value")));
            } catch (std::out_of_range&) {
                return createString("Invalid key or value");
            }
        }
        case CommandType::DEL: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
            } catch (std::out_of_range&) {
                return createString("Invalid key");
            }
            return operations::del(table, keyInt);
        }

        case CommandType::EXISTS: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
            } catch (std::out_of_range&) {
                return createString("Invalid key");
            }
            return operations::exists(table, keyInt);
        }

        case CommandType::KEYS: {
            return operations::keys(table);
        }

        case CommandType::TYPE: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
            } catch (std::out_of_range&) {
                return createString("Invalid key");
            }
            return operations::type(table, keyInt);
        }

        case CommandType::APPEND: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
                return operations::string::append(table, keyInt, command.args.at("value"));
            } catch (std::out_of_range&) {
                return make_error("Invalid key or value");
            }
        }

        case CommandType::STRLEN: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
            } catch (std::out_of_range&) {
                return make_error("Invalid key");
            }
            return operations::string::strlen(table, keyInt);
        }

        case CommandType::GETRANGE: {
            int keyInt, start, end;
            try {
                keyInt = command.args.at("key");
                start = command.args.at("start");
                end = command.args.at("end");
            } catch (std::out_of_range&) {
                return make_error("Invalid key or range");
            }
            return operations::string::getrange(table, keyInt, start, end);
        }

        case CommandType::LPUSH: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
                return operations::list::lpush(table, keyInt, JsonToDataType(command.args.at("value")));
            } catch (std::out_of_range&) {
                return make_error("Invalid key or value");
            }
        }

        case CommandType::RPUSH: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
                return operations::list::rpush(table, keyInt, JsonToDataType(command.args.at("value")));
            } catch (std::out_of_range&) {
                return make_error("Invalid key or value");
            }
        }

        case CommandType::LPOP: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
            } catch (std::out_of_range&) {
                return make_error("Invalid key");
            }
            return operations::list::lpop(table, keyInt);
        }

        case CommandType::RPOP: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
            } catch (std::out_of_range&) {
                return make_error("Invalid key");
            }
            return operations::list::rpop(table, keyInt);
        }

        case CommandType::LLEN: {
            int keyInt;
            try {
                keyInt = command.args.at("key");
            } catch (std::out_of_range&) {
                return make_error("Invalid key");
            }
            return operations::list::llen(table, keyInt);
        }

        case CommandType::LSET: {
            int keyInt, index;
            try {
                keyInt = command.args.at("key");
                index = command.args.at("index");
                return operations::list::lset(table, keyInt, index, JsonToDataType(command.args.at("value")));
            } catch (std::out_of_range&) {
                return make_error("Invalid key, index or value");
            }
        }
        
        case CommandType::LREM: {
            int keyInt, count;
            try {
                keyInt = command.args.at("key");
                count = command.args.at("count");
                return operations::list::lrem(table, keyInt, count, JsonToDataType(command.args.at("value")));
            } catch (std::out_of_range&) {
                return make_error("Invalid key, count or value");
            }
        }


    }

    return std::make_shared<Error>("Invalid command");
}
