#include "sortedset_operations.hpp"
#include "../datatypes/Sortedset.hpp"
#include <stdexcept>

namespace operations {
    namespace sortedset {
        void zadd(Table* table, const uint32_t key, const DataType* value, const float score) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            Sortedset* sortedset = dynamic_cast<Sortedset*>(table->get(key).get());
            if (sortedset == nullptr) throw std::invalid_argument("Value is not a sorted set");

            std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*) {});
            sortedset->zadd(value_ptr, score);
        }

        void zrem(Table* table, const uint32_t key, const DataType* value) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            Sortedset* sortedset = dynamic_cast<Sortedset*>(table->get(key).get());
            if (sortedset == nullptr) throw std::invalid_argument("Value is not a sorted set");

            std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*) {});
            sortedset->zrem(value_ptr);
        }

        std::vector<std::shared_ptr<DataType>> zrange(Table* table, const uint32_t key, const uint32_t start, const uint32_t end) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            Sortedset* sortedset = dynamic_cast<Sortedset*>(table->get(key).get());
            if (sortedset == nullptr) throw std::invalid_argument("Value is not a sorted set");

            return sortedset->zrange(start, end);
        }

        std::vector<std::shared_ptr<DataType>> zrevrange(Table* table, const uint32_t key, const uint32_t start, const uint32_t end) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            Sortedset* sortedset = dynamic_cast<Sortedset*>(table->get(key).get());
            if (sortedset == nullptr) throw std::invalid_argument("Value is not a sorted set");

            return sortedset->zrevrange(start, end);
        }

        uint32_t zcard(Table* table, const uint32_t key) {
            if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

            Sortedset* sortedset = dynamic_cast<Sortedset*>(table->get(key).get());
            if (sortedset == nullptr) throw std::invalid_argument("Value is not a sorted set");

            return sortedset->get_value().size();
        }
    }
}
