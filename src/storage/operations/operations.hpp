
#pragma once

#include <string>
#include <vector>
#include "../datatypes/Datatype.hpp"
#include "../datatypes/Geolocation.hpp"
#include "../../table/Table.hpp"

namespace operations {

    //global operations
    std::shared_ptr<DataType> get(Table* table, const uint32_t key);
    bool exists(Table* table, const uint32_t key);

    //string operations
    void append(Table* table, const uint32_t key, const std::string& value);
    uint32_t strlen(Table* table, const uint32_t key);

    //list operations
    void lpush(Table* table, const uint32_t key, const DataType* value);
    std::shared_ptr<DataType> lpop(Table* table, const uint32_t key);
    std::shared_ptr<DataType> rpop(Table* table, const uint32_t key);
    std::shared_ptr<DataType> lindex(Table* table, const uint32_t key, const uint32_t index);
    uint32_t llen(Table* table, const uint32_t key);
    void lset(Table* table, const uint32_t key, const uint32_t index, const DataType* value);
    void linsert(Table* table, const uint32_t key, const uint32_t index, const DataType* value);
    void lrem(Table* table, const uint32_t key, const uint32_t count, const DataType* value);

    //set operations
    void sadd(Table* table, const uint32_t key, const DataType* value);
    void srem(Table* table, const uint32_t key, const DataType* value);
    bool sismember(Table* table, const uint32_t key, const DataType* value);
    std::vector<DataType*> smembers(Table* table, const uint32_t key);

    //float operations
    //no specific operations, just use get and set

    //integer operations
    void increment(Table* table, const uint32_t key);
    void decrement(Table* table, const uint32_t key);
    void incrementby(Table* table, const uint32_t key, const int value);
    void decrementby(Table* table, const uint32_t key, const int value);

    //sorted set operations
    void zadd(Table* table, const uint32_t key, const DataType* value, const float score);
    void zrem(Table* table, const uint32_t key, const DataType* value);
    std::vector<std::shared_ptr<DataType>> zrange(Table* table, const uint32_t key, const uint32_t start, const uint32_t end);
    std::vector<std::shared_ptr<DataType>> zrevrange(Table* table, const uint32_t key, const uint32_t start, const uint32_t end);
    uint32_t zcard(Table* table, const uint32_t key);


    //geo operations
    float geodist(Table* table, const uint32_t key1, const uint32_t key2);
    std::vector<std::shared_ptr<Geolocation>> georadius(Table* table, const uint32_t key, const float latitude, const float longitude, const float radius);

    //bitmap operations
    void setbit(Table* table, const uint32_t key, const uint32_t offset, const bool value);
    bool getbit(Table* table, const uint32_t key, const uint32_t offset);
    uint32_t bitcount(Table* table, const uint32_t key);
    uint32_t bitpos(Table* table, const uint32_t key, const bool value);

}