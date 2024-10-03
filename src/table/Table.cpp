#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <cstddef>  // for std::byte
#include <utility>  // for std::move
#include <stdint.h>
#include <cstdint>
#include <tuple>
#include <memory>


#include "Table.hpp"

Table::Table(uint32_t initialSize){
    hashmap.reserve(initialSize);
}

Table::~Table() {
    clear();
}

void Table::reserve(uint32_t newSize) {
    hashmap.reserve(newSize);
}

void Table::clear() {
    hashmap.clear();
}

bool Table::exists(uint32_t key) {
    return hashmap.find(key) != hashmap.end();
}

std::shared_ptr<DataType> Table::get(uint32_t key) {
    if (!exists(key)) {
        throw std::invalid_argument("Key does not exist");
    }
    return hashmap[key];
}
