
#include <iostream>
#include <string>

#include "CommandExecutor.hpp"
#include "../Global.hpp"

void ExecuteCommand(int clientFd, const std::string& command, const nlohmann::json& parameters) {

    if (command == "ping") {
        writeResponse(clientFd, {{"response", "pong"}});
        return;
    }

    if (command == "create_table") {
        uint16_t tableId = storage->createTable(TABLE_INITIAL_SIZE);
        writeResponse(clientFd, {{"response", "table_created"}, {"table_id", tableId}});
        return;
    }

    if (command == "list_tables") {
            auto tables = storage->listTables();
            nlohmann::json response = {{"response", "tables_list"}, {"tables", {}}}; // Initialize with empty array
            if (tables.empty()) {
                response["tables"] = nlohmann::json::array();
            } else {
                for (const auto& [id, table] : tables) {
                    response["tables"].push_back({{"id", id}, {"size", table.getSize()}});
                }
            }
            writeResponse(clientFd, response);
            return;
        }

        if (command == "drop_table") {
            if (!parameters.contains("table_id")) {
            writeError(clientFd, "Missing 'table_id' parameter");
            return;
        }
        uint16_t tableId = parameters["table_id"];
        try {
            storage->dropTable(tableId);
            writeResponse(clientFd, {{"response", "table_dropped"}});
        } catch (const std::runtime_error& e) {
            writeError(clientFd, e.what());
        }
        return;

    }



    if (command == "add_element") {
        if (!parameters.contains("table_id") || !parameters.contains("key") || !parameters.contains("data")) {
            writeError(clientFd, "Missing 'table_id', 'key' or 'data' parameter");
            return;
        }
        uint16_t tableId = parameters["table_id"];
        uint32_t key = parameters["key"];

        std::vector<std::byte> data;
        //convert string to vector of bytes
        std::string dataStr = parameters["data"];
        for (const auto& byte : dataStr) {
            data.push_back(std::byte(byte));
        }

        try {
            storage->addElement(tableId, key, std::move(data));
            writeResponse(clientFd, {{"response", "element_added"}});
        } catch (const std::runtime_error& e) {
            writeError(clientFd, e.what());
        }
        return;
    }

    if (command == "remove_element") {
        if (!parameters.contains("table_id") || !parameters.contains("key")) {
            writeError(clientFd, "Missing 'table_id' or 'key' parameter");
            return;
        }
        uint16_t tableId = parameters["table_id"];
        uint32_t key = parameters["key"];
        try {
            storage->removeElement(tableId, key);
            writeResponse(clientFd, {{"response", "element_removed"}});
        } catch (const std::runtime_error& e) {
            writeError(clientFd, e.what());
        }
        return;
    }

    if (command == "set_element") {
        if (!parameters.contains("table_id") || !parameters.contains("key") || !parameters.contains("data")) {
            writeError(clientFd, "Missing 'table_id', 'key' or 'data' parameter");
            return;
        }
        uint16_t tableId = parameters["table_id"];
        uint32_t key = parameters["key"];

        std::vector<std::byte> data;
        //convert string to vector of bytes
        std::string dataStr = parameters["data"];
        for (const auto& byte : dataStr) {
            data.push_back(std::byte(byte));
        }

        try {
            storage->setElement(tableId, key, std::move(data));
            writeResponse(clientFd, {{"response", "element_set"}});
        } catch (const std::runtime_error& e) {
            writeError(clientFd, e.what());
        }
        return;
    }

    writeError(clientFd, "Unknown command");

}
