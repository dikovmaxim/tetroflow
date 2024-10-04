
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
#include "server/Server.hpp"
#include "log/Logger.hpp"
#include "storage/Storage.hpp"
#include "table/Table.hpp"

#include "storage/datatypes/Datatype.hpp"
#include "storage/datatypes/String.hpp"
#include "storage/datatypes/Integer.hpp"
#include "storage/datatypes/Set.hpp"
#include "storage/datatypes/Float.hpp"
#include "storage/datatypes/List.hpp"

#include "dml/parser.hpp"

#include "storage/datatypes/Iterable.hpp"

std::shared_ptr<Table> table;

std::shared_ptr<DataType> followIndexChain(std::vector<int> indexChain) {
    printf("Index chain: ");
    for (int index : indexChain) {
        printf("%d ", index);
    }
    printf("\n");

    if (indexChain.size() < 2) {
        throw std::invalid_argument("SET command requires at least 2 parts in the index chain");
    }

    // Get the table key
    uint32_t tableKey = indexChain[0];
    // Get the index key
    uint32_t indexKey = indexChain[1];
    std::vector<int> subIndexChain(indexChain.begin() + 2, indexChain.end());

    std::shared_ptr<DataType> currrentObject = table->get(indexKey);
    if (!currrentObject) {
        throw std::runtime_error("Failed to get initial object from table");
    }

    for (int i = 0; i < subIndexChain.size(); i++) {
        printf("Current subIndex: %d\n", subIndexChain[i]);
        if (std::dynamic_pointer_cast<Iterable>(currrentObject) != nullptr) {
            currrentObject = std::dynamic_pointer_cast<Iterable>(currrentObject)->get(subIndexChain[i]);
        } else {
            throw std::invalid_argument("Object is not iterable");
        }
    }

    return currrentObject;
}

void runCommand(Command command) {

    std::string commandName = command.command;
    std::vector<int> indexChain = command.indexChain;
    std::vector<std::string> types = command.types;
    std::vector<std::string> values = command.values;


    if (commandName == "GET") {

        std::shared_ptr<DataType> currrentObject;
        try {
            currrentObject = followIndexChain(indexChain);
        } catch (std::invalid_argument& e) {
            log(LOG_ERROR, e.what());
            return;
        }
        
        // Print the final object value
        if (currrentObject != nullptr) {
            log(LOG_INFO, "Final object value: " + currrentObject->to_string());
        } else {
            throw std::invalid_argument("Object not found");
        }

        return;
    }


    if (commandName == "SET") {

        printf("SET command\n");

        std::shared_ptr<DataType> currrentObject;
        try {
            currrentObject = followIndexChain(indexChain);
        } catch (std::invalid_argument& e) {
            log(LOG_ERROR, e.what());
            return;
        }

        std::vector<int> subIndexChain = std::vector<int>(indexChain.begin() + 2, indexChain.end());

        //check if there is at least one parameter
        if (values.size() < 1) {
            throw std::invalid_argument("SET command requires at least one parameter");
        }

        // Set the value to the object
        std::shared_ptr<DataType> value = from_string(values[0], types[0]);
        std::cout << "Value: " << value->to_string() << std::endl;

        if (std::dynamic_pointer_cast<Iterable>(currrentObject) != nullptr) {
            std::dynamic_pointer_cast<Iterable>(currrentObject)->set(subIndexChain[subIndexChain.size() - 1], value);
        } else {
            throw std::invalid_argument("Object " +DataTypeType_to_string(currrentObject->get_type())+ " is not iterable");
        }

        return;
    }

    //string operations
    if (commandName == "APPEND") {

        printf("APPEND command\n");

        std::shared_ptr<DataType> currrentObject;

        try {
            currrentObject = followIndexChain(indexChain);
        } catch (std::invalid_argument& e) {
            log(LOG_ERROR, e.what());
            return;
        }

        printf("Index chain finished\n");

        std::vector<int> subIndexChain = std::vector<int>(indexChain.begin() + 2, indexChain.end());

        //print the table
        table->print();

        //check if there is at least one parameter
        if (values.size() < 1) {
            throw std::invalid_argument("APPEND command requires at least one parameter");
        }

        // Set the value to the object
        std::shared_ptr<DataType> value = from_string(values[0], types[0]);
        std::shared_ptr<String> stringValue = std::dynamic_pointer_cast<String>(value);
        std::cout << "Value: " << stringValue->to_string() << std::endl;

        if (std::dynamic_pointer_cast<String>(currrentObject) != nullptr) {
            std::dynamic_pointer_cast<String>(currrentObject)->append(stringValue->get_value());
        } else {
            throw std::invalid_argument("Object " +DataTypeType_to_string(currrentObject->get_type())+ " is not a string");
        }


        return;
    }

}


int main(int argc, char** argv) {

    table = std::make_shared<Table>(10);

    std::shared_ptr<DataType> value = createString("Hello");
    table->set(0, value);

    std::shared_ptr<DataType> value2 = createString("World");
    table->set(1, value2);

    std::shared_ptr<DataType> value3 = createInteger(42);
    table->set(2, value3);

    std::shared_ptr<DataType> value4 = createInteger(69);
    table->set(3, value4);

    std::shared_ptr<DataType> value5 = createSet(std::vector<std::shared_ptr<DataType>>{value, value4});
    table->set(4, value5);

    std::shared_ptr<DataType> value6 = createFloat(3.14159);
    
    //create a list
    std::shared_ptr<DataType> value7 = createList(std::list<std::shared_ptr<DataType>>{value, value2, value3, value4, value5, value6});
    //table->set(0, value7);

    std::vector<std::string> commands = {
        "APPEND db[0].key[0] \" There, motherfucker!\""
    };

    for (const auto& cmd : commands) {
        CommandParser parser(cmd);
        runCommand(parser.getCommand());
        //print the command
        //parser.printCommand();
    }

    //print the table

        //print the table
    table->print();
    
    return 0;
}