
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

}