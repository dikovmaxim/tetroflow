
#include <string>
#include <vector>

#include "../Operations.hpp"
#include "../../datatypes/Datatype.hpp"
#include "../../datatypes/String.hpp"
#include "../../table/Table.hpp"
#include "../../datatypes/Error.hpp"
#include "../../datatypes/Integer.hpp"
#include "../../datatypes/List.hpp"

#include "../Operations.hpp"

namespace operations {
    
    namespace list {

        std::shared_ptr<DataType> lpush(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value) {
            std::shared_ptr<DataType> list = table->get(key);
            if (list == nullptr) {
                list = std::make_shared<List>();
                table->set(key, list);
            }

            if (list->get_type() != DataTypeType::LIST) {
                return make_error("WRONGTYPE Operation against a key holding the wrong kind of value");
            }

            std::shared_ptr<List> listPtr = std::static_pointer_cast<List>(list);
            listPtr->lpush(value);

            return createInteger(listPtr->size());
        }

        std::shared_ptr<DataType> rpush(std::shared_ptr<Table> table, int key, std::shared_ptr<DataType> value) {
            std::shared_ptr<DataType> list = table->get(key);
            if (list == nullptr) {
                list = std::make_shared<List>();
                table->set(key, list);
            }

            if (list->get_type() != DataTypeType::LIST) {
                return make_error("WRONGTYPE Operation against a key holding the wrong kind of value");
            }

            std::shared_ptr<List> listPtr = std::static_pointer_cast<List>(list);
            listPtr->rpush(value);

            return createInteger(listPtr->size());
        }

        std::shared_ptr<DataType> lpop(std::shared_ptr<Table> table, int key) {
            std::shared_ptr<DataType> list = table->get(key);
            if (list == nullptr) {
                return make_error("Key does not exist");
            }

            if (list->get_type() != DataTypeType::LIST) {
                return make_error("WRONGTYPE Operation against a key holding the wrong kind of value");
            }

            std::shared_ptr<List> listPtr = std::static_pointer_cast<List>(list);
            std::shared_ptr<DataType> value = listPtr->lpop();

            return value;
        }

        std::shared_ptr<DataType> rpop(std::shared_ptr<Table> table, int key) {
            std::shared_ptr<DataType> list = table->get(key);
            if (list == nullptr) {
                return make_error("Key does not exist");
            }

            if (list->get_type() != DataTypeType::LIST) {
                return make_error("WRONGTYPE Operation against a key holding the wrong kind of value");
            }

            std::shared_ptr<List> listPtr = std::static_pointer_cast<List>(list);
            std::shared_ptr<DataType> value = listPtr->rpop();

            return value;
        }

        std::shared_ptr<DataType> llen(std::shared_ptr<Table> table, int key) {
            std::shared_ptr<DataType> list = table->get(key);
            if (list == nullptr) {
                return createInteger(0);
            }

            if (list->get_type() != DataTypeType::LIST) {
                return make_error("WRONGTYPE Operation against a key holding the wrong kind of value");
            }

            std::shared_ptr<List> listPtr = std::static_pointer_cast<List>(list);
            return createInteger(listPtr->size());
        }

        std::shared_ptr<DataType> lset(std::shared_ptr<Table> table, int key, int index, std::shared_ptr<DataType> value) {
            std::shared_ptr<DataType> list = table->get(key);
            if (list == nullptr) {
                return make_error("Key does not exist");
            }

            if (list->get_type() != DataTypeType::LIST) {
                return make_error("WRONGTYPE Operation against a key holding the wrong kind of value");
            }

            std::shared_ptr<List> listPtr = std::static_pointer_cast<List>(list);
            if (index >= listPtr->size()) {
                return make_error("Index out of range");
            }

            listPtr->lset(index, value);

            return createString("OK");
        }

        std::shared_ptr<DataType> linsert(std::shared_ptr<Table> table, int key, int index, std::shared_ptr<DataType> value) {
            std::shared_ptr<DataType> list = table->get(key);
            if (list == nullptr) {
                return make_error("Key does not exist");
            }

            if (list->get_type() != DataTypeType::LIST) {
                return make_error("WRONGTYPE Operation against a key holding the wrong kind of value");
            }

            std::shared_ptr<List> listPtr = std::static_pointer_cast<List>(list);
            if (index > listPtr->size()) {
                return make_error("Index out of range");
            }

            listPtr->linsert(index, value);

            return createString("OK");
        }

        std::shared_ptr<DataType> lrem(std::shared_ptr<Table> table, int key, int count, std::shared_ptr<DataType> value) {
            std::shared_ptr<DataType> list = table->get(key);
            if (list == nullptr) {
                return createInteger(0);
            }

            if (list->get_type() != DataTypeType::LIST) {
                return make_error("WRONGTYPE Operation against a key holding the wrong kind of value");
            }

            std::shared_ptr<List> listPtr = std::static_pointer_cast<List>(list);
             listPtr->lrem(count, value);

            return createInteger(listPtr->size());
        }

    }

}