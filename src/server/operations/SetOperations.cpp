#include <string>
#include <vector>

#include "../Operations.hpp"
#include "../../datatypes/Datatype.hpp"
#include "../../datatypes/String.hpp"
#include "../../table/Table.hpp"
#include "../../datatypes/Error.hpp"
#include "../../datatypes/Integer.hpp"
#include "../../datatypes/Set.hpp"
#include "../../datatypes/Boolean.hpp"

#include "../Operations.hpp"

namespace operations {

    namespace dataset {

        std::shared_ptr<DataType> sadd(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value) {
            std::shared_ptr<DataType> data = table->get(key);
            if (data == nullptr || data->get_type() != DataTypeType::SET) throw std::invalid_argument("Key does not exist or hold the wrong kind of value");
            std::shared_ptr<Set> set = std::static_pointer_cast<Set>(data);
            if (set == nullptr) {
                set = std::make_shared<Set>();
                table->set(key, set);
            }
            set->sadd(value);
            return createBoolean(true);
        }

        std::shared_ptr<DataType> srem(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value) {
            std::shared_ptr<DataType> data = table->get(key);
            if (data == nullptr || data->get_type() != DataTypeType::SET) throw std::invalid_argument("Key does not exist or hold the wrong kind of value");
            std::shared_ptr<Set> set = std::static_pointer_cast<Set>(data);
            if (set == nullptr) {
                return std::make_shared<Integer>(0);
            }
            set->srem(value);
            return createBoolean(true);
        }
        
        std::shared_ptr<DataType> scard(std::shared_ptr<Table> table, int key) {
            std::shared_ptr<DataType> data = table->get(key);
            if (data == nullptr || data->get_type() != DataTypeType::SET) throw std::invalid_argument("Key does not exist or hold the wrong kind of value");
            std::shared_ptr<Set> set = std::static_pointer_cast<Set>(data);
            if (set == nullptr) {
                return std::make_shared<Integer>(0);
            }
            return std::make_shared<Integer>(set->scard());
        }

    }

}