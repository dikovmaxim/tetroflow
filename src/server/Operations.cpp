#include "Operations.hpp"
#include "../datatypes/Datatype.hpp"
#include "../table/Table.hpp"
#include "../datatypes/Response.hpp"

namespace operations {
    
    std::shared_ptr<DataType> get(std::shared_ptr<Table> table, int key) {
        return table->get(key);
    }
    
    std::shared_ptr<DataType> set(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value) {
        table->set(key, value);
        return value;
    }

    std::shared_ptr<DataType> del(std::shared_ptr<Table> table, int key) {
        table->remove(key);
        return createResponse("Key deleted");
    }
    
}