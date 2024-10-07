
#include <string>
#include <vector>

#include "../Operations.hpp"
#include "../../datatypes/Datatype.hpp"
#include "../../datatypes/String.hpp"
#include "../../table/Table.hpp"
#include "../../datatypes/Error.hpp"
#include "../../datatypes/Integer.hpp"

#include "../Operations.hpp"

namespace operations {
    
    namespace string {

        std::shared_ptr<DataType> append(std::shared_ptr<Table> table, int key, std::string value) {
            if (!table->exists(key)) {
                return make_error("Key does not exist");
            }
            std::shared_ptr<DataType> data = table->get(key);
            if (data->get_type() != DataTypeType::STRING) {
                return make_error("Operation against a key holding the wrong kind of value");
            }
            std::shared_ptr<String> str = std::static_pointer_cast<String>(data);
            str->append(value);
            return createString(str->get_value());
        }

        std::shared_ptr<DataType> strlen(std::shared_ptr<Table> table, int key) {
            if (!table->exists(key)) {
                return make_error("Key does not exist");
            }
            std::shared_ptr<DataType> data = table->get(key);
            if (data->get_type() != DataTypeType::STRING) {
                return make_error("Operation against a key holding the wrong kind of value");
            }
            std::shared_ptr<String> str = std::static_pointer_cast<String>(data);
            return createInteger(str->get_value().size());
        }

        std::shared_ptr<DataType> getrange(std::shared_ptr<Table> table, int key, int start, int end) {
            if (!table->exists(key)) {
                return make_error("Key does not exist");
            }
            std::shared_ptr<DataType> data = table->get(key);
            if (data->get_type() != DataTypeType::STRING) {
                return make_error("Operation against a key holding the wrong kind of value");
            }
            std::shared_ptr<String> str = std::static_pointer_cast<String>(data);
            std::string value = str->get_value();
            if (start < 0) {
                start = value.size() + start;
            }
            if (end < 0) {
                end = value.size() + end;
            }
            if (start < 0) {
                start = 0;
            }
            if (end < 0) {
                end = 0;
            }
            if (start > value.size()) {
                start = value.size();
            }
            if (end > value.size()) {
                end = value.size();
            }
            if (start > end) {
                return createString("");
            }
            return createString(value.substr(start, end - start + 1));
        }

    }

}