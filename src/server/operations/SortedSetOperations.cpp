
#include <string>
#include <vector>
#include <memory>

#include "../Operations.hpp"
#include "../../datatypes/Datatype.hpp"
#include "../../datatypes/String.hpp"
#include "../../table/Table.hpp"
#include "../../datatypes/Error.hpp"
#include "../../datatypes/Integer.hpp"
#include "../../datatypes/Set.hpp"
#include "../../datatypes/Sortedset.hpp"
#include "../../datatypes/List.hpp"
#include "../../datatypes/Boolean.hpp"

#include "../Operations.hpp"

namespace operations {
    
    namespace sortedset {

        std::shared_ptr<DataType> zadd(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value, const float score) {
            std::shared_ptr<Sortedset> sortedset = std::static_pointer_cast<Sortedset>(table->get(key));
            if (sortedset == nullptr) {
                sortedset = std::make_shared<Sortedset>();
                table->set(key, sortedset);
            }
            sortedset->zadd(value, score);
            return createBoolean(true);
        }

        std::shared_ptr<DataType> zrem(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value) {
            std::shared_ptr<Sortedset> sortedset = std::static_pointer_cast<Sortedset>(table->get(key));
            if (sortedset == nullptr) {
                return std::make_shared<Integer>(0);
            }
            sortedset->zrem(value);
            return createBoolean(true);
        }
        
        std::shared_ptr<DataType> zrange(std::shared_ptr<Table> table, int key, int start, int end) {
            std::shared_ptr<Sortedset> sortedset = std::static_pointer_cast<Sortedset>(table->get(key));
            if (sortedset == nullptr) {
                return std::make_shared<List>();
            }
            std::vector<std::shared_ptr<DataType>> result = sortedset->zrange(start, end);
            std::shared_ptr<List> list = std::make_shared<List>();
            for (auto& item : result) {
                list->rpush(item);
            }
            return list;
        }

        std::shared_ptr<DataType> zrevrange(std::shared_ptr<Table> table, int key, int start, int end) {
            std::shared_ptr<Sortedset> sortedset = std::static_pointer_cast<Sortedset>(table->get(key));
            if (sortedset == nullptr) {
                return std::make_shared<List>();
            }
            std::vector<std::shared_ptr<DataType>> result = sortedset->zrevrange(start, end);
            std::shared_ptr<List> list = std::make_shared<List>();
            for (auto& item : result) {
                list->rpush(item);
            }
            return list;
        }

        std::shared_ptr<DataType> zcard(std::shared_ptr<Table> table, int key) {
            std::shared_ptr<Sortedset> sortedset = std::static_pointer_cast<Sortedset>(table->get(key));
            if (sortedset == nullptr) {
                return std::make_shared<Integer>(0);
            }
            return std::make_shared<Integer>(sortedset->zcard());
        }


    }
}