#include <vector>
#include <stdexcept>
#include <cstddef>  // for std::byte
#include <utility>  // for std::move
#include <stdint.h>
#include <cstdint>
#include <tuple>

#include "Storage.hpp"
#include "../table/Table.hpp"

Storage::Storage(){
    tables.reserve(3);
}

Storage::~Storage() {
    tables.clear();
}

uint16_t Storage::createTable(uint32_t initialSize) {
    uint16_t tableId = tables.size();
    tables.emplace(tableId, Table(initialSize));
    return tableId;
}

void Storage::dropTable(uint16_t tableId) {
    tables.erase(tableId);
}

Table Storage::getTable(uint16_t tableId) {
    return tables.at(tableId);
}

