#pragma once

#include "string_operations.hpp"
#include "list_operations.hpp"
#include "set_operations.hpp"
#include "int_operations.hpp"
#include "geo_operations.hpp"
#include "sortedset_operations.hpp"
#include "bitmap_operations.hpp"

namespace operations {
    // Other global operations can stay here
    std::shared_ptr<DataType> get(Table* table, const uint32_t key);
    bool exists(Table* table, const uint32_t key);
}