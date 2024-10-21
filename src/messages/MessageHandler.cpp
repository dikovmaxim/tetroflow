#include "MessageHandler.hpp"

#include "../log/Logger.hpp"
#include "Message.hpp"
#include "MessageUtils.hpp"
#include "../server/json_fwd.hpp"
#include "../gossip/GossipManager.hpp"
#include "../gossip/nodes/ClientNode.hpp"
#include "../transactions/Transaction.hpp"
#include "../transactions/TransactionManager.hpp"
#include "../table/Table.hpp"
#include "../server/Command.hpp"
#include "../Global.hpp"
#include "messagetypes/MessageReplicate.hpp"

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <thread>

std::condition_variable messageCv; // Condition variable for message queue
bool stopMessageHandling = false; // Flag to stop message handling
std::mutex messageQueueMutex; // Mutex for message queue

std::queue<std::shared_ptr<Message>> messages; // Message queue
std::vector<std::shared_ptr<Message>> messageLog; // Message log

std::thread messageHandlingThread; // Thread for handling messages

void EmptyCallback(const std::shared_ptr<DataType>, Client&) {}

std::shared_ptr<Client> emptyClient = std::make_shared<Client>();

std::shared_ptr<Transaction> createTransaction(std::shared_ptr<MessageReplicate> message) {
    std::shared_ptr<Transaction> transaction = std::make_shared<Transaction>(coreTable, EmptyCallback, *emptyClient);
    for (auto command : message->getCommands()) {
        transaction->addCommand(command);
    }
    return transaction;
}

void addMessageToExchangeQueue(std::shared_ptr<Message> message) {
    if(messageInList(message)) {
        return;
    }

    if(message->GetType() == MessageType::MESSAGE_REPLICATE) {
        std::shared_ptr<MessageReplicate> replicateMessage = std::dynamic_pointer_cast<MessageReplicate>(message);
        std::shared_ptr<Transaction> transaction = createTransaction(replicateMessage);
        addTransaction(transaction);
    }

    {
        std::lock_guard<std::mutex> lock(messageQueueMutex);
        messages.push(message);
    }
    messageCv.notify_one(); // Notify the handling thread
    messageLog.push_back(message);
}

void messageExchangeQueue() {
    log(LOG_INFO, "Starting message exchange");
    while (true) {
        std::unique_lock<std::mutex> lock(messageQueueMutex);
        messageCv.wait(lock, [] { 
            return !messages.empty() || stopMessageHandling; 
        });
        
        if (stopMessageHandling) {
            return; // Exit loop when stop flag is set
        }
        
        while (!messages.empty()) {
            std::shared_ptr<Message> message = messages.front();
            messages.pop();
            
            if (message) {
                broadcastMessage(message);
            }
        }
    }
}


void startMessageExchangeQueue() {
    messageHandlingThread = std::thread(messageExchangeQueue);
    messageHandlingThread.detach();
}

void stopMessageExchangeQueue() {
    stopMessageHandling = true;
    messageCv.notify_one();
}

bool messageInList(std::shared_ptr<Message> message) {
    //starting from the back of the list, because we put the most recent messages at the back, and we have to do less comparisons
    for (auto it = messageLog.rbegin(); it != messageLog.rend(); ++it) {
        if ((*it)->compare(message)) {
            return true;
        }
    }
    return false;
}