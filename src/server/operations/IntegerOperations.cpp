
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
    
    namespace integer {

        std::shared_ptr<DataType> inc(std::shared_ptr<Table> table, int key) {
            std::shared_ptr<DataType> data = table->get(key);
            if (data == nullptr) {
                table->set(key, std::make_shared<Integer>(1));
                return std::make_shared<Integer>(1);
            }
            if (data->get_type() != DataTypeType::INTEGER) {
                return make_error("ERR value is not an integer or out of range");
            }
            std::shared_ptr<Integer> integer = std::static_pointer_cast<Integer>(data);
            integer->set_value(integer->get_value() + 1);
            return std::make_shared<Integer>(integer->get_value());
        }

        std::shared_ptr<DataType> dec(std::shared_ptr<Table> table, int key) {
            std::shared_ptr<DataType> data = table->get(key);
            if (data == nullptr) {
                table->set(key, std::make_shared<Integer>(-1));
                return std::make_shared<Integer>(-1);
            }
            if (data->get_type() != DataTypeType::INTEGER) {
                return make_error("ERR value is not an integer or out of range");
            }
            std::shared_ptr<Integer> integer = std::static_pointer_cast<Integer>(data);
            integer->set_value(integer->get_value() - 1);
            return std::make_shared<Integer>(integer->get_value());
        }

        std::shared_ptr<DataType> incby(std::shared_ptr<Table> table, int key, int increment) {
            std::shared_ptr<DataType> data = table->get(key);
            if (data == nullptr) {
                table->set(key, std::make_shared<Integer>(increment));
                return std::make_shared<Integer>(increment);
            }
            if (data->get_type() != DataTypeType::INTEGER) {
                return make_error("ERR value is not an integer or out of range");
            }
            std::shared_ptr<Integer> integer = std::static_pointer_cast<Integer>(data);
            integer->set_value(integer->get_value() + increment);
            return std::make_shared<Integer>(integer->get_value());
        }

        std::shared_ptr<DataType> decby(std::shared_ptr<Table> table, int key, int decrement) {
            std::shared_ptr<DataType> data = table->get(key);
            if (data == nullptr) {
                table->set(key, std::make_shared<Integer>(-decrement));
                return std::make_shared<Integer>(-decrement);
            }
            if (data->get_type() != DataTypeType::INTEGER) {
                return make_error("ERR value is not an integer or out of range");
            }
            std::shared_ptr<Integer> integer = std::static_pointer_cast<Integer>(data);
            integer->set_value(integer->get_value() - decrement);
            return std::make_shared<Integer>(integer->get_value());
        }

    }

}