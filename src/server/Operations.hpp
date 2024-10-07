
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

    

    

}