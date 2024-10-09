
#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "Transaction.hpp"
#include "TransactionManager.hpp"
#include "../log/Logger.hpp"

std::queue<std::shared_ptr<Transaction>> transactions; // Shared transaction queue
std::mutex queueMutex; // Mutex for protecting the transaction queue
std::condition_variable cv; // Condition variable for transaction queue
bool stopHandling = false; // Flag to stop transaction handling

std::thread handlingThread; // Thread for handling transactions

void addTransaction(std::shared_ptr<Transaction> transaction) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        transactions.push(transaction);
    }
    cv.notify_one(); // Notify the handling thread
}


void handleQueue() {
    log(LOG_INFO, "Starting transaction handling");
    while (true) {
        std::shared_ptr<Transaction> transaction = nullptr;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [] { return !transactions.empty() || stopHandling; });
            if (stopHandling && transactions.empty()) {
                break;
            }
            if (!transactions.empty()) {
                transaction = transactions.front();
                transactions.pop();
            }
        }
        if (transaction) {
            transaction->commit();
        }
    }
}

void startTransactionHandling() {
    handlingThread = std::thread(handleQueue);
    handlingThread.detach();
}

void stopTransactionHandling() {
    stopHandling = true;
    cv.notify_one();
    handlingThread.join();
    log(LOG_INFO, "Stopped transaction handling");
}