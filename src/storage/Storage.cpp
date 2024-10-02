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
    uint16_t id = tables.size();
    tables.emplace(id, Table(initialSize));
    return id;
}

void Storage::dropTable(uint16_t id) {
    auto it = tables.find(id);
    if (it == tables.end()) {
        throw std::runtime_error("Table does not exist");
    }
    tables.erase(it);
}

void Storage::addElement(uint16_t tableId, uint32_t key, std::vector<std::byte>&& data) {
    auto it = tables.find(tableId);
    if (it == tables.end()) {
        throw std::runtime_error("Table does not exist");
    }
    it->second.addElement(key, std::move(data));
}

void Storage::removeElement(uint16_t tableId, uint32_t key) {
    auto it = tables.find(tableId);
    if (it == tables.end()) {
        throw std::runtime_error("Table does not exist");
    }
    it->second.removeElement(key);
}

void Storage::changeElement(uint16_t tableId, uint32_t key, std::vector<std::byte>&& newData) {
    auto it = tables.find(tableId);
    if (it == tables.end()) {
        throw std::runtime_error("Table does not exist");
    }
    it->second.changeElement(key, std::move(newData));
}

void Storage::setElement(uint16_t tableId, uint32_t key, std::vector<std::byte>&& data) {
    auto it = tables.find(tableId);
    if (it == tables.end()) {
        throw std::runtime_error("Table does not exist");
    }
    it->second.setElement(key, std::move(data));
}

std::vector<std::byte>& Storage::getElement(uint16_t tableId, uint32_t key) {
    auto it = tables.find(tableId);
    if (it == tables.end()) {
        throw std::runtime_error("Table does not exist");
    }
    return it->second.getElement(key);
}

std::vector<std::tuple<uint32_t, Table>> Storage::listTables() {
    std::vector<std::tuple<uint32_t, Table>> result;
    for (auto& [id, table] : tables) {
        result.push_back(std::make_tuple(id, table));
    }
    return result;
}

Table Storage::getTable(uint16_t id) {
    auto it = tables.find(id);
    if (it == tables.end()) {
        throw std::runtime_error("Table does not exist");
    }
    return it->second;
}