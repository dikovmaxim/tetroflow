#pragma once
#include "../../table/Table.hpp"

namespace operations {
    namespace bitmap {
        void setbit(Table* table, const uint32_t key, const uint32_t offset, const bool value);
        bool getbit(Table* table, const uint32_t key, const uint32_t offset);
        uint32_t bitcount(Table* table, const uint32_t key);
        uint32_t bitpos(Table* table, const uint32_t key, const bool value);
    }
}
