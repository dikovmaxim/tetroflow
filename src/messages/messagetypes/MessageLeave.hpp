#include "../Message.hpp"
#include "../MessageTypes.hpp"
#include "../../server/json_fwd.hpp"

#include <string>
#include <exception>

//message sent when a node joins the network

class MessageLeave : public Message {
public:
    std::string node_id;

    MessageLeave(){
        type = MessageType::MESSAGE_LEAVE;
        this->PopulateNewMessage();
    }

    nlohmann::json ToJson() const override {
        nlohmann::json j;
        j["type"] = MessageTypeToString(type);
        j["content"]["node_id"] = node_id;
        j["content"]["unique_id"] = GetUniqueId();
        j["content"]["timestamp"] = GetTimestamp();
        return j;
    }

    void FromJson(const nlohmann::json& j) override {
        try
        {
            node_id = j.at("content").at("node_id").get<std::string>();
            SetContent(j.at("content"));
            SetUniqueId(j.at("content").at("unique_id").get<int>());
            SetTimestamp(j.at("content").at("timestamp").get<int>());
        }
        catch (std::exception& e)
        {
            throw std::invalid_argument("Invalid JSON for message leave");
        }
    }

    std::string ToString() const override {
        return "Leave message from node " + node_id;
    }

    MessageType GetType() const override {
        return type;
    }

    ~MessageLeave() override = default;
};