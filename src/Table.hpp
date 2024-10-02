#pragma once

#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <cstddef>  // for std::byte
#include <utility>  // for std::move
#include <cstdint>

class Table {
private:
    std::unordered_map<uint32_t, std::vector<std::byte>> hashmap;
    void reserve(uint32_t newSize);
    void clear();
    uint16_t id;

public:

    Table(uint16_t id, uint32_t initialSize);
    ~Table();

    void addElement(uint32_t key, std::vector<std::byte>&& data);
    void removeElement(uint32_t key);
    void changeElement(uint32_t key, std::vector<std::byte>&& newData);
    uint16_t getId();

};