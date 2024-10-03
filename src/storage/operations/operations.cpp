#include "operations.hpp"

#include <unordered_map>
#include <vector>
#include <list>
#include <stdexcept>
#include <cstddef>  // for std::byte
#include <utility>  // for std::move
#include <stdint.h>
#include <cstdint>
#include <tuple>
#include <memory>
#include <cmath>
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <vector>
#include <memory>


#include "../datatypes/Datatype.hpp"
#include "../datatypes/Geolocation.hpp"
#include "../datatypes/String.hpp"
#include "../datatypes/List.hpp"
#include "../datatypes/Set.hpp"
#include "../datatypes/Integer.hpp"
#include "../datatypes/Sortedset.hpp"

namespace operations {

    //global operations
    std::shared_ptr<DataType> get(Table* table, const uint32_t key) {
        return table->get(key);
    }

    

    bool exists(Table* table, const uint32_t key) {
        return table->exists(key);
    }

    //string operations

    void append(Table* table, const uint32_t key, const std::string& value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        String* str = dynamic_cast<String*>(table->get(key).get());
        if (str == nullptr) throw std::invalid_argument("Value is not a string");

        str->append(value);
    }

    uint32_t strlen(Table* table, const uint32_t key) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        String* str = dynamic_cast<String*>(table->get(key).get());
        if (str == nullptr) throw std::invalid_argument("Value is not a string");

        return str->get_value().size();
    }


    //list operations
    void lpush(Table* table, const uint32_t key, const DataType* value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        List* list = dynamic_cast<List*>(table->get(key).get());
        if (list == nullptr) throw std::invalid_argument("Value is not a list");

        //create a shared pointer to the value, without cloning it
        std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*){});

        list->lpush(value_ptr);
    }

    void rpush(Table* table, const uint32_t key, const DataType* value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        List* list = dynamic_cast<List*>(table->get(key).get());
        if (list == nullptr) throw std::invalid_argument("Value is not a list");

        //create a shared pointer to the value, without cloning it
        std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*){});

        list->rpush(value_ptr);
    }

    std::shared_ptr<DataType> lpop(Table* table, const uint32_t key) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        List* list = dynamic_cast<List*>(table->get(key).get());
        if (list == nullptr) throw std::invalid_argument("Value is not a list");

        return list->lpop();
    }

    std::shared_ptr<DataType> rpop(Table* table, const uint32_t key) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        List* list = dynamic_cast<List*>(table->get(key).get());
        if (list == nullptr) throw std::invalid_argument("Value is not a list");

        return list->rpop();
    }

    std::shared_ptr<DataType> lindex(Table* table, const uint32_t key, const uint32_t index) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        List* list = dynamic_cast<List*>(table->get(key).get());
        if (list == nullptr) throw std::invalid_argument("Value is not a list");

        return list->lindex(index);
    }

    uint32_t llen(Table* table, const uint32_t key) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        List* list = dynamic_cast<List*>(table->get(key).get());
        if (list == nullptr) throw std::invalid_argument("Value is not a list");

        return list->get_value().size();
    }

    void lset(Table* table, const uint32_t key, const uint32_t index, const DataType* value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        List* list = dynamic_cast<List*>(table->get(key).get());
        if (list == nullptr) throw std::invalid_argument("Value is not a list");

        //create a shared pointer to the value, without cloning it
        std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*){});

        auto it = list->get_value().begin();
        std::advance(it, index);
        *it = value_ptr;
    }

    void linsert(Table* table, const uint32_t key, const uint32_t index, const DataType* value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        List* list = dynamic_cast<List*>(table->get(key).get());
        if (list == nullptr) throw std::invalid_argument("Value is not a list");

        //create a shared pointer to the value, without cloning it
        std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*){});

        auto it = list->get_value().begin();
        std::advance(it, index);
        list->get_value().insert(it, value_ptr);
    }

    void lrem(Table* table, const uint32_t key, const uint32_t count, const DataType* value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        List* list = dynamic_cast<List*>(table->get(key).get());
        if (list == nullptr) throw std::invalid_argument("Value is not a list");

        //create a shared pointer to the value, without cloning it
        std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*){});

        list->get_value().remove(value_ptr);
    }

    //set operations

    void sadd(Table* table, const uint32_t key, const DataType* value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Set* set = dynamic_cast<Set*>(table->get(key).get());
        if (set == nullptr) throw std::invalid_argument("Value is not a set");

        //create a shared pointer to the value, without cloning it
        std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*){});

        set->sadd(value_ptr);
    }

    void srem(Table* table, const uint32_t key, const DataType* value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Set* set = dynamic_cast<Set*>(table->get(key).get());
        if (set == nullptr) throw std::invalid_argument("Value is not a set");

        //create a shared pointer to the value, without cloning it
        std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*){});

        set->srem(value_ptr);
    }

    bool sismember(Table* table, const uint32_t key, const DataType* value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Set* set = dynamic_cast<Set*>(table->get(key).get());
        if (set == nullptr) throw std::invalid_argument("Value is not a set");

        //create a shared pointer to the value, without cloning it
        std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*){});

        return set->sismember(value_ptr);
    }

    std::vector<DataType*> smembers(Table* table, const uint32_t key) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Set* set = dynamic_cast<Set*>(table->get(key).get());
        if (set == nullptr) throw std::invalid_argument("Value is not a set");

        std::vector<DataType*> members;
        for (auto it = set->get_value().begin(); it != set->get_value().end(); it++) {
            std::shared_ptr<DataType> member = *it;
            members.push_back(member.get());
        }
        return members;
    }

    //float operations
    //no specific operations, just use get and set

    //integer operations
    void increment(Table* table, const uint32_t key) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Integer* integer = dynamic_cast<Integer*>(table->get(key).get());
        if (integer == nullptr) throw std::invalid_argument("Value is not an integer");

        integer->increment();
    }

    void decrement(Table* table, const uint32_t key) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Integer* integer = dynamic_cast<Integer*>(table->get(key).get());
        if (integer == nullptr) throw std::invalid_argument("Value is not an integer");

        integer->decrement();
    }

    void incrementby(Table* table, const uint32_t key, const int value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Integer* integer = dynamic_cast<Integer*>(table->get(key).get());
        if (integer == nullptr) throw std::invalid_argument("Value is not an integer");

        integer->incrementby(value);
    }

    void decrementby(Table* table, const uint32_t key, const int value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Integer* integer = dynamic_cast<Integer*>(table->get(key).get());
        if (integer == nullptr) throw std::invalid_argument("Value is not an integer");

        integer->decrementby(value);
    }


    //sorted set operations
    void zadd(Table* table, const uint32_t key, const DataType* value, const float score) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Sortedset* sortedset = dynamic_cast<Sortedset*>(table->get(key).get());
        if (sortedset == nullptr) throw std::invalid_argument("Value is not a sorted set");

        //create a shared pointer to the value, without cloning it
        std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*){});

        sortedset->zadd(value_ptr, score);
    }

    void zrem(Table* table, const uint32_t key, const DataType* value) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Sortedset* sortedset = dynamic_cast<Sortedset*>(table->get(key).get());
        if (sortedset == nullptr) throw std::invalid_argument("Value is not a sorted set");

        //create a shared pointer to the value, without cloning it
        std::shared_ptr<DataType> value_ptr(const_cast<DataType*>(value), [](DataType*){});

        sortedset->zrem(value_ptr);
    }

    std::vector<std::shared_ptr<DataType>> zrange(Table* table, const uint32_t key, const uint32_t start, const uint32_t end) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Sortedset* sortedset = dynamic_cast<Sortedset*>(table->get(key).get());
        if (sortedset == nullptr) throw std::invalid_argument("Value is not a sorted set");

        return sortedset->zrange(start, end);
    }

    std::vector<std::shared_ptr<DataType>> zrevrange(Table* table, const uint32_t key, const uint32_t start, const uint32_t end) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Sortedset* sortedset = dynamic_cast<Sortedset*>(table->get(key).get());
        if (sortedset == nullptr) throw std::invalid_argument("Value is not a sorted set");

        return sortedset->zrevrange(start, end);
    }

    uint32_t zcard(Table* table, const uint32_t key) {
        if (!table->exists(key)) throw std::invalid_argument("Key does not exist");

        Sortedset* sortedset = dynamic_cast<Sortedset*>(table->get(key).get());
        if (sortedset == nullptr) throw std::invalid_argument("Value is not a sorted set");

        return sortedset->get_value().size();
    }


    //geo operations
    float geodist(Table* table, const uint32_t key1, const uint32_t key2) {
        if (!table->exists(key1) || !table->exists(key2)) throw std::invalid_argument("Key does not exist");

        Geolocation* geo1 = dynamic_cast<Geolocation*>(table->get(key1).get());
        Geolocation* geo2 = dynamic_cast<Geolocation*>(table->get(key2).get());
        if (geo1 == nullptr || geo2 == nullptr) throw std::invalid_argument("Value is not a geolocation");

        float lat1 = geo1->get_latitude();
        float lng1 = geo1->get_longitude();
        float lat2 = geo2->get_latitude();
        float lng2 = geo2->get_longitude();

        return sqrt(pow(lat1 - lat2, 2) + pow(lng1 - lng2, 2));
    }



}