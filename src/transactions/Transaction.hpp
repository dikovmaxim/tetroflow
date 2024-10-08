#pragma once

#include <memory>
#include <vector>
#include <list>
#include <string>
#include <stdexcept>
#include <functional>

#include "../table/Table.hpp"
#include "../server/Command.hpp"
#include "../server/Operations.hpp"
#include "../datatypes/DataType.hpp"


typedef struct TransactionError {
    std::string message;
    Command command;
} TransactionError;

class Transaction
{
private:
    std::list<Command> commands;
    std::shared_ptr<Table> table;
    std::shared_ptr<Table> snapshot;
    TransactionError error;
    std::shared_ptr<DataType> result;
    std::function<void(const std::shared_ptr<DataType>&, int)> callback;

    bool isCommited;
    int socket;
    //callback function with a 
public:
    Transaction(std::shared_ptr<Table> table, std::function<void(const std::shared_ptr<DataType>&, int)> callback, int socket);
    ~Transaction();
    void addCommand(Command command);
    void commit();
    void rollback();
    TransactionError getError();
};

Transaction makeSingleCommandTransaction(
    Command command,
    std::shared_ptr<Table> table,
    std::function<void(
        const std::shared_ptr<DataType>&,
        int)>
    callback,
    int socket
);