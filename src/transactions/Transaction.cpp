#include "Transaction.hpp"

#include "../server/Command.hpp"
#include "../server/Operations.hpp"

#include <memory>
#include <stdexcept>


Transaction::Transaction(std::shared_ptr<Table> table) {
    this->snapshot = table;
    this->table = table->copy();
    this->isCommited = false;
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
            return;
        }
    }
    isCommited = true;
    snapshot->merge(table);
    snapshot->printBeautify();
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

Transaction makeSingleCommandTransaction(Command command, std::shared_ptr<Table> table) {
    Transaction transaction(table);
    transaction.addCommand(command);
    return transaction;
}