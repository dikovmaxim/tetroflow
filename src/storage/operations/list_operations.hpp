#pragma once
#include "../../table/Table.hpp"
#include "../datatypes/Datatype.hpp"

namespace operations {
    namespace list {
        void lpush(Table* table, const uint32_t key, const DataType* value);
        std::shared_ptr<DataType> lpop(Table* table, const uint32_t key);
        std::shared_ptr<DataType> rpop(Table* table, const uint32_t key);
        std::shared_ptr<DataType> lindex(Table* table, const uint32_t key, const uint32_t index);
        uint32_t llen(Table* table, const uint32_t key);
        void lset(Table* table, const uint32_t key, const uint32_t index, const DataType* value);
        void linsert(Table* table, const uint32_t key, const uint32_t index, const DataType* value);
        void lrem(Table* table, const uint32_t key, const uint32_t count, const DataType* value);
    }
}
