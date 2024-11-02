#include "ClusterPipe.hpp"

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
}

ClusterPipeState ClusterPipe::getState() {
    return state;
}

