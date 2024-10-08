
#include <memory>
#include <vector>
#include <queue>
#include <string>

#include "Transaction.hpp"
#include "TransactionManager.hpp"
#include "../log/Logger.hpp"

std::queue<std::shared_ptr<Transaction>> transactions;

void addTransaction(std::shared_ptr<Transaction> transaction) {
    transactions.push(transaction);
}

void startTransactionHandling() {
    log(LOG_INFO, "Starting transaction handling");
    while (true) {
        if (!transactions.empty()) {
            std::shared_ptr<Transaction> transaction = transactions.front();
            transactions.pop();
            transaction->commit();
        }
    }
}