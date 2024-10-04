#pragma once
#include "../../table/Table.hpp"
#include "../datatypes/Datatype.hpp"
#include <vector>

namespace operations {
    namespace sortedset {
        void zadd(Table* table, const uint32_t key, const DataType* value, const float score);
        void zrem(Table* table, const uint32_t key, const DataType* value);
        std::vector<std::shared_ptr<DataType>> zrange(Table* table, const uint32_t key, const uint32_t start, const uint32_t end);
        std::vector<std::shared_ptr<DataType>> zrevrange(Table* table, const uint32_t key, const uint32_t start, const uint32_t end);
        uint32_t zcard(Table* table, const uint32_t key);
    }
}
