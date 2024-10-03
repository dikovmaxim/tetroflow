#pragma once

#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <cstddef>  // for std::byte
#include <utility>  // for std::move
#include <cstdint>
#include <tuple>
#include <memory>

#include "../storage/datatypes/Datatype.hpp"

class Table {
private:
    std::unordered_map<uint32_t, std::shared_ptr<DataType>> hashmap;
    void reserve(uint32_t newSize);
    void clear();

public:

    Table(uint32_t initialSize);
    ~Table();
    bool exists(uint32_t key);
    std::shared_ptr<DataType> get(uint32_t key);
    void set(uint32_t key, std::shared_ptr<DataType> value);
    void remove(uint32_t key);

    void print();
};