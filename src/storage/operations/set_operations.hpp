#pragma once
#include "../../table/Table.hpp"
#include "../datatypes/Datatype.hpp"
#include <vector>

namespace operations {
    namespace set {
        void sadd(Table* table, const uint32_t key, const DataType* value);
        void srem(Table* table, const uint32_t key, const DataType* value);
        bool sismember(Table* table, const uint32_t key, const DataType* value);
        std::vector<DataType*> smembers(Table* table, const uint32_t key);
    }
}
