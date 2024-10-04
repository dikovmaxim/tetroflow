#pragma once
#include "../../table/Table.hpp"
#include "../datatypes/Geolocation.hpp"
#include <vector>

namespace operations {
    namespace geo {
        float geodist(Table* table, const uint32_t key1, const uint32_t key2);
    }
}
