#include "list_operations.hpp"
#include "../datatypes/List.hpp"
#include <stdexcept>
#include <iterator>

namespace operations {
    namespace list {
        void lpush(Table* table, const uint32_t key, const DataType* value) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            List* list = dynamic_cast<List*>(table->get(key).get());
            if (list == nullptr) throw std::invalid_argument("Value is not a list");

            std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*) {});
            list->lpush(value_ptr);
        }

        std::shared_ptr<DataType> lpop(Table* table, const uint32_t key) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            List* list = dynamic_cast<List*>(table->get(key).get());
            if (list == nullptr) throw std::invalid_argument("Value is not a list");

            return list->lpop();
        }

        std::shared_ptr<DataType> rpop(Table* table, const uint32_t key) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            List* list = dynamic_cast<List*>(table->get(key).get());
            if (list == nullptr) throw std::invalid_argument("Value is not a list");

            return list->rpop();
        }

        std::shared_ptr<DataType> lindex(Table* table, const uint32_t key, const uint32_t index) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            List* list = dynamic_cast<List*>(table->get(key).get());
            if (list == nullptr) throw std::invalid_argument("Value is not a list");

            return list->lindex(index);
        }

        uint32_t llen(Table* table, const uint32_t key) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            List* list = dynamic_cast<List*>(table->get(key).get());
            if (list == nullptr) throw std::invalid_argument("Value is not a list");

            return list->get_value().size();
        }

        void lset(Table* table, const uint32_t key, const uint32_t index, const DataType* value) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            List* list = dynamic_cast<List*>(table->get(key).get());
            if (list == nullptr) throw std::invalid_argument("Value is not a list");

            std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*) {});
            auto it = list->get_value().begin();
            std::advance(it, index);
            *it = value_ptr;
        }
    }
}
