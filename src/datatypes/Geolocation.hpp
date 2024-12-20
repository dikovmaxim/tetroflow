#pragma once

#include <string>

#include "Datatype.hpp"

class Geolocation : public DataType {
public:
    Geolocation(float latitude, float longitude) : latitude(latitude), longitude(longitude) {}
    Geolocation() : latitude(0), longitude(0) {}


    float get_latitude() const {
        return latitude;
    }

    void set_latitude(float latitude) {
        this->latitude = latitude;
    }

    float get_longitude() const {
        return longitude;
    }

    void set_longitude(float longitude) {
        this->longitude = longitude;
    }

    std::shared_ptr<DataType> copy() const override {
        return std::make_shared<Geolocation>(latitude, longitude);
    }

    DataTypeType get_type() const override {
        return DataTypeType::GEO;
    }

    std::string to_string() const override {
        return "(\"Lat\": "+std::to_string(latitude) + "), (\"Lng\": " + std::to_string(longitude) + ")";
    }

    bool operator==(const DataType& other) const override {
        if (get_type() != other.get_type()) {
            return false;
        }
        Geolocation otherGeolocation = static_cast<const Geolocation&>(other);
        return latitude == otherGeolocation.get_latitude() && longitude == otherGeolocation.get_longitude();
    }   

private:
    float latitude;
    float longitude;
};