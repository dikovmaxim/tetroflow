#include "Transaction.hpp"

#include "../server/Command.hpp"
#include "../server/Operations.hpp"
#include "../datatypes/Datatype.hpp"
#include "../datatypes/Error.hpp"

#include "../log/Logger.hpp"
#include "../server/Client.hpp"

#include <memory>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>


Transaction::Transaction(std::shared_ptr<Table> table, std::function<void(const std::shared_ptr<DataType>, Client&)> callback, Client& client)
    : client(client) {  // Initialize client in the initializer list
    this->snapshot = table;
    this->table = table->copy();
    this->isCommited = false;
    this->callback = callback;
}

void Transaction::addCommand(Command command) {
    commands.push_back(command);
}

void Transaction::commit() {
    for (Command command : commands) {
        try {
            result = executeCommand(command, table);
        } catch (const std::exception& e) {
            error.command = command;
            error.message = e.what();
            rollback();
            callback(std::make_shared<Error>(std::string("An error occurred during the execution of the command. Error: ") + e.what()), client);
        }
    }
    if (client.getSocket() > 0)
    {
        callback(result, client);
    }
    
    isCommited = true;
    snapshot->merge(table);
    
}

void Transaction::rollback() {
    table = snapshot->copy();
}

TransactionError Transaction::getError() {
    return error;
}

Transaction::~Transaction() {
    if (!isCommited) {
        rollback();
    }
}

std::vector<Command> Transaction::getCommands() {
    return std::vector<Command>(commands.begin(), commands.end());
}

Transaction makeSingleCommandTransaction(
    Command command,
    std::shared_ptr<Table> table,
    std::function<void(const std::shared_ptr<DataType>, Client&)> callback,
    Client& client
) {
    Transaction transaction(table, callback, client);
    transaction.addCommand(command);
    return transaction;
}

