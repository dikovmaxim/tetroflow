
#include "MessageParser.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "../server/json_fwd.hpp"

#include "Message.hpp"
#include "MessageUtils.hpp"
#include "MessageTypes.hpp"

#include "messagetypes/MessageJoin.hpp"
#include "messagetypes/MessageLeave.hpp"
#include "messagetypes/MessageReplicate.hpp"


std::shared_ptr<Message> parseMessage(nlohmann::json message){
    try
    {
        MessageType type = MessageTypeFromString(message["type"]);
        switch (type)
        {
            case MessageType::MESSAGE_JOIN:
            {
                MessageJoin joinMessage = MessageJoin("");
                joinMessage.FromJson(message);
                return std::make_shared<MessageJoin>(joinMessage);
            }
            
            case MessageType::MESSAGE_REPLICATE:
            {
                MessageReplicate replicateMessage = MessageReplicate();
                replicateMessage.FromJson(message);
                return std::make_shared<MessageReplicate>(replicateMessage);
            }

            default:
                throw std::invalid_argument("Invalid message type");
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        throw std::invalid_argument("Invalid message");
    }
}