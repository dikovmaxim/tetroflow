#pragma once
#include <string>
#include "../../table/Table.hpp"

namespace operations {
    namespace string {
        void append(Table* table, const uint32_t key, const std::string& value);
        uint32_t strlen(Table* table, const uint32_t key);
    }
}
