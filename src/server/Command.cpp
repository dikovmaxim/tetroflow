
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../Global.hpp"
#include "../datatypes/Datatype.hpp"
#include "json_fwd.hpp"


#include "Command.hpp"
#include "Operations.hpp"

#include "../datatypes/Datatype.hpp"
#include "../datatypes/Response.hpp"
#include "../datatypes/Set.hpp"
#include "../datatypes/List.hpp"
#include "../datatypes/Integer.hpp"
#include "../datatypes/Float.hpp"
#include "../datatypes/String.hpp"

Command jsonToCommand(std::string json) {
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

    // Get the arguments from the JSON object
    // Ensure that if the argument is an array, it gets treated as a single List
    if (j["arguments"].is_array()) {
        command.args.push_back(j["arguments"]);  // Pass the array as a whole
    } else {
        for (auto& arg : j["arguments"]) {
            command.args.push_back(arg);
        }
    }

    return command;
}



void printCommand(Command command){
    for (auto& arg : command.args) {
        std::cout << arg << std::endl;
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

    // Get the arguments
    std::vector<nlohmann::json> args = command.args;

    // Get the JSON object
    nlohmann::json j = command.json;

    std::cout << "Executing command: " << j << std::endl;

    // Execute the command based on the type
    switch (type) {
        case CommandType::GET: {
            int keyInt;
            try {
                keyInt = j["key"];
            } catch (std::invalid_argument& e) {
                return createResponse("Invalid key");
            }
            return operations::get(table, keyInt);
        }
        case CommandType::SET: {
            int keyInt;
            try {
                keyInt = j["key"];
            } catch (std::invalid_argument& e) {
                return createResponse("Invalid key");
            }
            return operations::set(table, keyInt, JsonToDataType(args[0]));
        }
        case CommandType::DEL: {
            int keyInt;
            try {
                keyInt = j["key"];
            } catch (std::invalid_argument& e) {
                return createResponse("Invalid key");
            }
            return operations::del(table, keyInt);
        }
        default:
            break;
    }

    return std::make_shared<Response>("Command not implemented");  return std::make_shared<Response>("Command not implemented");
}

