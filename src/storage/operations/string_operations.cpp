#include "string_operations.hpp"
#include "../datatypes/String.hpp"
#include <stdexcept>

namespace operations {
    namespace string {
        void append(Table* table, const uint32_t key, const std::string& value) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            String* str = dynamic_cast<String*>(table->get(key).get());
            if (str == nullptr) throw std::invalid_argument("Value is not a string");

            str->append(value);
        }

        uint32_t strlen(Table* table, const uint32_t key) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            String* str = dynamic_cast<String*>(table->get(key).get());
            if (str == nullptr) throw std::invalid_argument("Value is not a string");

            return str->get_value().size();
        }
    }
}
