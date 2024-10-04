#pragma once
#include "../../table/Table.hpp"

namespace operations {
    namespace integer {
        void increment(Table* table, const uint32_t key);
        void decrement(Table* table, const uint32_t key);
        void incrementby(Table* table, const uint32_t key, const int value);
        void decrementby(Table* table, const uint32_t key, const int value);
    }
}
