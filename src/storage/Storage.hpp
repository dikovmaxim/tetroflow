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

    void addElement(uint16_t tableId, uint32_t key, std::vector<std::byte>&& data);
    void removeElement(uint16_t tableId, uint32_t key);
    void changeElement(uint16_t tableId, uint32_t key, std::vector<std::byte>&& newData);
    void setElement(uint16_t tableId, uint32_t key, std::vector<std::byte>&& data);

    Table getTable(uint16_t id);

    std::vector<std::tuple<uint32_t, Table>> listTables();

    std::vector<std::byte>& getElement(uint16_t tableId, uint32_t key);

};