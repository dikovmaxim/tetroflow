#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <cstddef>  // for std::byte
#include <utility>  // for std::move
#include <stdint.h>
#include <cstdint>
#include <tuple>
#include <memory>
#include <iostream>


#include "Table.hpp"
#include "../datatypes/Datatype.hpp"

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

void Table::set(uint32_t key, std::shared_ptr<DataType> value) {
    hashmap[key] = value;
}

void Table::remove(uint32_t key) {
    if (!exists(key)) {
        throw std::invalid_argument("Key does not exist");
    }
    hashmap.erase(key);
}

std::vector<uint32_t> Table::getKeys() {
    std::vector<uint32_t> keys;
    for (auto const& [key, val] : hashmap) {
        keys.push_back(key);
    }
    return keys;
}

void Table::print() {
    for (auto const& [key, val] : hashmap) {
        std::cout << key << ": " << val->to_string() << std::endl;
    }
}

void Table::printBeautify() {
    printf("Table:\n");
    printf("--------------------\n");
    printf("Key\tValue\tType\n");
    printf("--------------------\n");
    for (auto const& [key, val] : hashmap) {
        printf("%d\t%s\t%s\n", key, val->to_string().c_str(), DataTypeType_to_string(val->get_type()).c_str());
    }
    printf("--------------------\n");
}

std::shared_ptr<Table> Table::copy() {
    auto newTable = std::make_shared<Table>(hashmap.size());
    for (auto const& [key, val] : hashmap) {
        newTable->set(key, val->copy());
    }
    return newTable;
}

void Table::merge(std::shared_ptr<Table> other) {
    for (auto const& [key, val] : other->hashmap) {
        hashmap[key] = val;
    }
}