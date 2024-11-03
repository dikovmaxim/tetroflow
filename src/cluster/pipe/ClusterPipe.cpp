#include "ClusterPipe.hpp"
#include "../../messages/Message.hpp"

void ClusterPipe::setOnMessage(std::function<void(std::shared_ptr<std::vector<std::byte>>)> onMessage) {
    this->onMessage = onMessage;
}

void ClusterPipe::setOnStateChange(std::function<void(ClusterPipeState)> onStateChange) {
    this->onStateChange = onStateChange;
}

void ClusterPipe::setState(ClusterPipeState state) {
    this->state = state;
    if (onStateChange) {
        onStateChange(state);
    }
    //if state is disconnected, destroy the thread
    if (state == ClusterPipeState::DISCONNECTED) {
        running = false; // WARNING: This code is redundant and should be managed from the inheriting classes
        if (queueWorkerThread.joinable()) {
            queueWorkerThread.join();
        }
    }

    //if state is connected, create the thread
    if (state == ClusterPipeState::CONNECTED) {
        running = true; // WARNING: This code is redundant and should be managed from the inheriting classes
        queueWorkerThread = std::thread(&ClusterPipe::processQueue, this);
    }

}

ClusterPipeState ClusterPipe::getState() {
    return state;
}

void ClusterPipe::addMessageToQueue(std::shared_ptr<Message> value){
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(value);
    }
    cv.notify_one(); // Notify the handling thread
}

void ClusterPipe::processQueue() {
    while (true) {
        std::shared_ptr<Message> message = nullptr;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !queue.empty() || !running; });

            if (!running) {
                break;
            }

            if (!queue.empty()) {
                message = queue.front();
                queue.pop();
            }
        }
        if (message) {
            this->sendBytes(message->serialize());
        }
    }
}