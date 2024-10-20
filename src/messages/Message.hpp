#pragma once

#include "MessageTypes.hpp"
#include "../server/json_fwd.hpp"
#include "MessageUtils.hpp"

#include <string>
#include <chrono>


//abstract class for messages being sent over the gossip network

class Message {
public:
    MessageType type;

    virtual nlohmann::json ToJson() const = 0;
    virtual void FromJson(const nlohmann::json& j) = 0;
    virtual std::string ToString() const = 0;
    virtual MessageType GetType() const = 0;

    virtual ~Message() = default;

    void PopulateNewMessage() {
        this->unique_id = generate_unique_id();
        this->timestamp = generate_unique_timestamp();
    }

    int GetUniqueId() const {
        return unique_id;
    }

    int GetTimestamp() const {
        return timestamp;
    }

    void SetContent(const nlohmann::json& j) {
        content = j;
    }

    void SetUniqueId(int id) {
        unique_id = id;
    }

    void SetTimestamp(int time) {
        timestamp = generate_unique_timestamp();
    }

    bool compare(std::shared_ptr<Message> message) {
        return (this->unique_id == message->GetUniqueId() && this->timestamp == message->GetTimestamp());
    }


private:
    nlohmann::json content;
    int unique_id;
    int timestamp;
};