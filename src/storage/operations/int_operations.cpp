#include "int_operations.hpp"
#include "../datatypes/Integer.hpp"
#include <stdexcept>

namespace operations {
    namespace integer {
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
    }
}
