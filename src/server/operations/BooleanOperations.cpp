
#include <string>
#include <vector>
#include <memory>

#include "../Operations.hpp"
#include "../../datatypes/Datatype.hpp"
#include "../../datatypes/String.hpp"
#include "../../table/Table.hpp"
#include "../../datatypes/Error.hpp"
#include "../../datatypes/Integer.hpp"
#include "../../datatypes/Set.hpp"
#include "../../datatypes/Sortedset.hpp"
#include "../../datatypes/List.hpp"
#include "../../datatypes/Boolean.hpp"

#include "../Operations.hpp"

namespace operations {
    
    namespace boolean {

        std::shared_ptr<DataType> flip(std::shared_ptr<Table> table, int key) {
            std::shared_ptr<DataType> data = table->get(key);
            if (data == nullptr) {
                return make_error("Key does not exist");
            }
            if (data->get_type() != DataTypeType::BOOLEAN) {
                return make_error("Operation against a key holding the wrong kind of value");
            }
            std::shared_ptr<Boolean> boolean = std::static_pointer_cast<Boolean>(data);
            boolean->flip();
            return createBoolean(boolean->get_value());
        }

    }

}