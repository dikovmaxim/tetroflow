
#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <mutex>

#include "Transaction.hpp"
#include "TransactionManager.hpp"
#include "../log/Logger.hpp"

std::queue<std::shared_ptr<Transaction>> transactions; // Shared transaction queue
std::mutex queue_mutex; // Mutex to protect the queue during insertion and removal

void addTransaction(std::shared_ptr<Transaction> transaction) {
    std::lock_guard<std::mutex> lock(queue_mutex); // Lock the mutex for insertion
    transactions.push(transaction);
}

void startTransactionHandling() {
    log(LOG_INFO, "Starting transaction handling");
    while (true) {
        std::shared_ptr<Transaction> transaction = nullptr;

        // Only lock the queue when accessing it
        {
            std::lock_guard<std::mutex> lock(queue_mutex); // Lock the mutex to access the queue
            if (!transactions.empty()) {
                transaction = transactions.front();
                transactions.pop();
            }
        }

        // Commit the transaction outside the locked scope
        if (transaction) {
            transaction->commit();
        }
    }
}