
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../Global.hpp"
#include "../datatypes/Datatype.hpp"


namespace operations {
    
    std::shared_ptr<DataType> get(std::shared_ptr<Table> table, int key);
    std::shared_ptr<DataType> set(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value);
    std::shared_ptr<DataType> del(std::shared_ptr<Table> table, int key);
    std::shared_ptr<DataType> exists(std::shared_ptr<Table> table, int key);
    std::shared_ptr<DataType> keys(std::shared_ptr<Table> table);
    std::shared_ptr<DataType> type(std::shared_ptr<Table> table, int key);


    namespace string {

        std::shared_ptr<DataType> append(std::shared_ptr<Table> table, int key, std::string value);

        std::shared_ptr<DataType> strlen(std::shared_ptr<Table> table, int key);

        std::shared_ptr<DataType> getrange(std::shared_ptr<Table> table, int key, int start, int end);

    }

    namespace list {

        std::shared_ptr<DataType> lpush(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value);

        std::shared_ptr<DataType> rpush(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value);

        std::shared_ptr<DataType> lpop(std::shared_ptr<Table> table, int key);

        std::shared_ptr<DataType> rpop(std::shared_ptr<Table> table, int key);

        std::shared_ptr<DataType> llen(std::shared_ptr<Table> table, int key);

        std::shared_ptr<DataType> lset(std::shared_ptr<Table> table, int key, int index, std::shared_ptr<DataType> value);

        std::shared_ptr<DataType> linsert(std::shared_ptr<Table> table, int key, int index, std::shared_ptr<DataType> value);

        std::shared_ptr<DataType> lrem(std::shared_ptr<Table> table, int key, int count, std::shared_ptr<DataType> value);

    }

    
    namespace dataset {

        std::shared_ptr<DataType> sadd(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value);

        std::shared_ptr<DataType> srem(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value);

        std::shared_ptr<DataType> smembers(std::shared_ptr<Table> table, int key);

        std::shared_ptr<DataType> sismember(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value);

        std::shared_ptr<DataType> scard(std::shared_ptr<Table> table, int key);

    }

    namespace sortedset {

        std::shared_ptr<DataType> zadd(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value, float score);

        std::shared_ptr<DataType> zrem(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value);

        std::shared_ptr<DataType> zrange(std::shared_ptr<Table> table, int key, int start, int end);

        std::shared_ptr<DataType> zrevrange(std::shared_ptr<Table> table, int key, int start, int end);

        std::shared_ptr<DataType> zcard(std::shared_ptr<Table> table, int key);

    }

    namespace integer {

        std::shared_ptr<DataType> inc(std::shared_ptr<Table> table, int key);

        std::shared_ptr<DataType> dec(std::shared_ptr<Table> table, int key);

        std::shared_ptr<DataType> incby(std::shared_ptr<Table> table, int key, int increment);

        std::shared_ptr<DataType> decby(std::shared_ptr<Table> table, int key, int decrement);

    }

    namespace boolean {

        std::shared_ptr<DataType> flip(std::shared_ptr<Table> table, int key);
    }

}