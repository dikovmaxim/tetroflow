
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
#include <thread>

#include "Global.hpp"
#include "log/Logger.hpp"
#include "table/Table.hpp"

#include "datatypes/Datatype.hpp"
#include "transactions/Transaction.hpp"
#include "transactions/TransactionManager.hpp"

#include "server/Command.hpp"


void transactionCallback(const std::shared_ptr<DataType>& result, int socket) {
    //std::cout << "Transaction callback: " << result->to_string() << std::endl;
}

void test(){


    //wait 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));
    coreTable -> printBeautify();

    Command command = jsonToCommand(R"({
        "command": "INC",
        "arguments": {"key": 1}
    })");

    for(int i = 0; i < 100; i++){
        Transaction transaction = makeSingleCommandTransaction(command, coreTable, transactionCallback, i);
        addTransaction(std::make_shared<Transaction>(transaction));
        //std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }

    std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    coreTable -> printBeautify();
    
}

int main(int argc, char** argv) {

    initStorage();
    startTransactionHandling();

    Command command = jsonToCommand(R"({
        "command": "SET",
        "arguments": {"key": 1, "value": 100}
    })");

    Transaction transaction = makeSingleCommandTransaction(command, coreTable, transactionCallback, 0);
    addTransaction(std::make_shared<Transaction>(transaction));


    std::thread testThread(test);
    testThread.detach();

    return 0;
}