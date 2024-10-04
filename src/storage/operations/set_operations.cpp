#include "set_operations.hpp"
#include "../datatypes/Set.hpp"
#include <stdexcept>

namespace operations {
    namespace set {
        void sadd(Table* table, const uint32_t key, const DataType* value) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            Set* set = dynamic_cast<Set*>(table->get(key).get());
            if (set == nullptr) throw std::invalid_argument("Value is not a set");

            std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*) {});
            set->sadd(value_ptr);
        }

        void srem(Table* table, const uint32_t key, const DataType* value) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            Set* set = dynamic_cast<Set*>(table->get(key).get());
            if (set == nullptr) throw std::invalid_argument("Value is not a set");

            std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*) {});
            set->srem(value_ptr);
        }

        bool sismember(Table* table, const uint32_t key, const DataType* value) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            Set* set = dynamic_cast<Set*>(table->get(key).get());
            if (set == nullptr) throw std::invalid_argument("Value is not a set");

            std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*) {});
            return set->sismember(value_ptr);
        }

        std::vector<DataType*> smembers(Table* table, const uint32_t key) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            Set* set = dynamic_cast<Set*>(table->get(key).get());
            if (set == nullptr) throw std::invalid_argument("Value is not a set");

            std::vector<DataType*> members;
            for (auto it = set->get_value().begin(); it != set->get_value().end(); ++it) {
                std::shared_ptr<DataType> member = *it;
                members.push_back(member.get());
            }
            return members;
        }
    }
}
