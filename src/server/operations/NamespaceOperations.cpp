
#include <string>
#include <vector>
#include <memory>

#include "../Operations.hpp"
#include "../../datatypes/Datatype.hpp"
#include "../../datatypes/String.hpp"
#include "../../table/Table.hpp"
#include "../../datatypes/Error.hpp"
#include "../../datatypes/List.hpp"
#include "../../datatypes/Json.hpp"

#include "../../gossip/GossipManager.hpp"


namespace operations {
    
    namespace service {

        std::shared_ptr<DataType> ping() {
            return createString("PONG");
        }

        std::shared_ptr<DataType> getnodes() {
            std::shared_ptr<List> list = std::make_shared<List>();
            std::vector<std::shared_ptr<Node>> nodes = getNodes();
            for (auto& node : nodes) {
                list->rpush(createJson(node->toJson()));
            }
            return list;
        }
        
    }

}