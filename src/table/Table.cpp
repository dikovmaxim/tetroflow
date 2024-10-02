#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <cstddef>  // for std::byte
#include <utility>  // for std::move
#include <stdint.h>
#include <cstdint>
#include <tuple>


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

void Table::setElement(uint32_t key, std::vector<std::byte>&& data) {
    //if element on key exists, change it, otherwise add new element
    auto it = hashmap.find(key);
    if (it == hashmap.end()) {
        hashmap.emplace(key, std::move(data));
    } else {
        it->second = std::move(data);
    }
}

int Table::getSize() const{
    return hashmap.size();
}

std::vector<std::byte>& Table::getElement(uint32_t key) {
    auto it = hashmap.find(key);
    if (it == hashmap.end()) {
        throw std::runtime_error("Element does not exist");
    }
    return it->second;
}

std::vector<std::tuple<uint32_t, std::vector<std::byte>>> Table::listElements() {
    std::vector<std::tuple<uint32_t, std::vector<std::byte>>> elements;
    for (const auto& [key, data] : hashmap) {
        elements.push_back({key, data});
    }
    return elements;
}
