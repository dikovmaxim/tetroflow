#include "Transaction.hpp"

#include "../server/Command.hpp"
#include "../server/Operations.hpp"
#include "../datatypes/DataType.hpp"
#include "../datatypes/Error.hpp"

#include <memory>
#include <stdexcept>


Transaction::Transaction(std::shared_ptr<Table> table, std::function<void(const std::shared_ptr<DataType>&, int)> callback, int socket) {
    this->snapshot = table;
    this->table = table->copy();
    this->isCommited = false;
    this->socket = socket;
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
            callback(std::make_shared<Error>(error.message), socket);
            return;
        }
    }
    isCommited = true;
    snapshot->merge(table);
    snapshot->printBeautify();
    callback(result, socket);
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

Transaction makeSingleCommandTransaction(
    Command command,
    std::shared_ptr<Table> table,
    std::function<void(
        const std::shared_ptr<DataType>&,
        int)>
    callback,
    int socket
) {
    Transaction transaction(table, callback, socket);
    transaction.addCommand(command);
    return transaction;
}