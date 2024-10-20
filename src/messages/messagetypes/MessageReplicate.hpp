#include "../Message.hpp"
#include "../MessageTypes.hpp"
#include "../../server/json_fwd.hpp"
#include "../../server/Command.hpp"
#include "../../transactions/Transaction.hpp"

#include <string>
#include <exception>

//message sent when a node joins the network

class MessageReplicate : public Message {
public:
    
    std::vector<Command> commands;

    MessageReplicate(){
        type = MessageType::MESSAGE_REPLICATE;
        this->PopulateNewMessage();
    }

    nlohmann::json ToJson() const override {
        nlohmann::json j;
        j["type"] = MessageTypeToString(type);
        j["unique_id"] = GetUniqueId();
        j["timestamp"] = GetTimestamp();
        j["content"]["commands"] = nlohmann::json::array();
        for (auto command : commands){
            j["content"]["commands"].push_back(command.json);
        }

        return j;
    }

    void FromJson(const nlohmann::json& j) override {
        std::cout << j.dump() << " is a replicate message" << std::endl;
        try
        {
            SetUniqueId(j.at("unique_id").get<int>());
            SetTimestamp(j.at("timestamp").get<int>());
            for (const auto& command : j.at("content").at("commands")) {
                std::cout << command.dump() << " is a command" << std::endl;
                //Command jsonToCommand(const std::string& json) {
                Command c = jsonToCommand(command.dump());
                commands.push_back(c);
                std::cout << c.json.dump() << " is a parsed command" << std::endl;
            }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            throw std::invalid_argument("Invalid JSON for message replicate");
        }
    }

    std::string ToString() const override {
        return "Server replication message with commands " + std::to_string(commands.size());
    }

    MessageType GetType() const override {
        return type;
    }

    ~MessageReplicate() override = default;
};

inline std::shared_ptr<Message> createReplicateMessage(Transaction transaction){
    std::shared_ptr<MessageReplicate> message = std::make_shared<MessageReplicate>();
    message->commands = transaction.getCommands();
    return message;
}