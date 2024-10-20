#include "MessageHandler.hpp"

#include "../log/Logger.hpp"
#include "Message.hpp"
#include "MessageUtils.hpp"
#include "../server/json_fwd.hpp"
#include "../gossip/GossipManager.hpp"

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

std::thread messageHandlingThread; // Thread for handling messages

void addMessageToExchangeQueue(std::shared_ptr<Message> message) {
    {
        std::lock_guard<std::mutex> lock(messageQueueMutex);
        messages.push(message);
    }
    messageCv.notify_one(); // Notify the handling thread
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

        log(LOG_INFO, "Handling message"); // Log every time after waking up
        
        while (!messages.empty()) {
            std::shared_ptr<Message> message = messages.front();
            messages.pop();
            
            if (message) {
                log(LOG_INFO, "Message: " + message->ToString());
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
    std::lock_guard<std::mutex> lock(messageQueueMutex);
    for (int i = 0; i < messages.size(); i++) {
        if (messages.front() == message) {
            return true;
        }
    }
    return false;
}