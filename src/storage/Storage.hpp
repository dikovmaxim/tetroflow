#pragma once

#include <vector>
#include <unordered_map>
#include <tuple>

#include "../table/Table.hpp"

class Storage {

    std::unordered_map<uint16_t, Table> tables;

public:
    Storage();
    ~Storage();

    uint16_t createTable(uint32_t initialSize);
    void dropTable(uint16_t id);
    Table getTable(uint16_t id);

};