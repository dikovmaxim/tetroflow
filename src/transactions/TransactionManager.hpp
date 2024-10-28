#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <string>

#include "Transaction.hpp"

void addTransaction(std::shared_ptr<Transaction> transaction);
void startTransactionHandling();
void stopTransactionHandling();
