#include "geo_operations.hpp"
#include "../datatypes/Geolocation.hpp"
#include <cmath>
#include <stdexcept>

namespace operations {
    namespace geo {
        float geodist(Table* table, const uint32_t key1, const uint32_t key2) {
            if (!table->exists(key1) || !table->exists(key2)) throw std::invalid_argument("Key does not exist");

            Geolocation* geo1 = dynamic_cast<Geolocation*>(table->get(key1).get());
            Geolocation* geo2 = dynamic_cast<Geolocation*>(table->get(key2).get());
            if (geo1 == nullptr || geo2 == nullptr) throw std::invalid_argument("Value is not a geolocation");

            float lat1 = geo1->get_latitude();
            float lng1 = geo1->get_longitude();
            float lat2 = geo2->get_latitude();
            float lng2 = geo2->get_longitude();

            return sqrt(pow(lat1 - lat2, 2) + pow(lng1 - lng2, 2));
        }

    }


}
