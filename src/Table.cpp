#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <cstddef>  // for std::byte
#include <utility>  // for std::move
#include <stdint.h>
#include <cstdint>

#include "Table.hpp"

Table::Table(uint16_t id, uint32_t initialSize){
    hashmap.reserve(initialSize);
    this->id = id;
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

void Table::addElement(uint32_t key, std::vector<std::byte>&& data) {
    if (hashmap.find(key) != hashmap.end()) {
            throw std::runtime_error("Element with this key already exists");
    }
    hashmap.emplace(key, std::move(data));  // Use emplace and move to avoid copying
}

void Table::removeElement(uint32_t key) {
    auto it = hashmap.find(key);
    if (it == hashmap.end()) {
        throw std::runtime_error("Element does not exist");
    }
    hashmap.erase(it);  // Automatically handles memory cleanup
}

void Table::changeElement(uint32_t key, std::vector<std::byte>&& newData) {
    auto it = hashmap.find(key);
    if (it == hashmap.end()) {
        throw std::runtime_error("Element does not exist");
    }
    it->second = std::move(newData);  // Use move to avoid copying
}

