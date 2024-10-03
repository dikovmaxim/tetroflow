
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <string>
#include <list>

#include "Global.hpp"
#include "server/Server.hpp"
#include "log/Logger.hpp"
#include "storage/Storage.hpp"
#include "table/Table.hpp"

#include "storage/datatypes/Datatype.hpp"
#include "storage/datatypes/String.hpp"
#include "storage/datatypes/Integer.hpp"
#include "storage/datatypes/Set.hpp"
#include "storage/datatypes/Float.hpp"
#include "storage/datatypes/List.hpp"


int main(int argc, char** argv) {

    initStorage();

    Table table = Table(0);

    std::shared_ptr<DataType> value = createString("Hello");
    table.set(0, value);

    std::shared_ptr<DataType> value2 = createString("World");
    table.set(1, value2);

    std::shared_ptr<DataType> value3 = createInteger(42);
    table.set(2, value3);

    std::shared_ptr<DataType> value4 = createInteger(69);
    table.set(3, value4);

    std::shared_ptr<DataType> value5 = createSet(std::vector<std::shared_ptr<DataType>>{value, value4});
    table.set(4, value5);

    std::shared_ptr<DataType> value6 = createFloat(3.14159);
    
    //create a list
    std::shared_ptr<DataType> value7 = createList(std::list<std::shared_ptr<DataType>>{value, value2, value3, value4, value5, value6});
    table.set(5, value7);

    table.print();


    
    while (true) {}


    return 0;
}