#include "operations.hpp"
#include "../datatypes/Datatype.hpp"
#include "../../table/Table.hpp"

namespace operations {
    std::shared_ptr<DataType> get(Table* table, const uint32_t key) {
        return table->get(key);
    }

    bool exists(Table* table, const uint32_t key) {
        return table->exists(key);
    }
}
