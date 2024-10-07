#include "Operations.hpp"
#include "../datatypes/Datatype.hpp"
#include "../table/Table.hpp"
#include "../datatypes/Boolean.hpp"
#include "../datatypes/List.hpp"
#include "../datatypes/Integer.hpp"
#include "../datatypes/String.hpp"
#include "../datatypes/Error.hpp"

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
        return createString("OK");
    }

    std::shared_ptr<DataType> exists(std::shared_ptr<Table> table, int key) {
        return createBoolean(table->exists(key));
    }

    std::shared_ptr<DataType> keys(std::shared_ptr<Table> table) {
        std::vector<uint32_t> keys = table->getKeys();
        std::shared_ptr<List> list = std::make_shared<List>();
        for (uint32_t key : keys) {
            list->lpush(createInteger(key));
        }
        return list;
    }

    std::shared_ptr<DataType> type(std::shared_ptr<Table> table, int key) {
        if (!table->exists(key)) {
            return make_error("Key does not exist");
        }
        std::shared_ptr<DataType> value = table->get(key);
        return createString(DataTypeType_to_string(value->get_type()));
    }
    
}