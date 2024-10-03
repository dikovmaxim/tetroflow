#pragma once

#include <string>

#include "Datatype.hpp"

class Geolocation : public DataType {
public:
    Geolocation(float latitude, float longitude) : latitude(latitude), longitude(longitude) {}

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

    DataTypeType get_type() const override {
        return DataTypeType::GEO;
    }

    std::string to_string() const override {
        return "(\"Lat\": "+std::to_string(latitude) + "), (\"Lng\": " + std::to_string(longitude) + ")";
    }

private:
    float latitude;
    float longitude;
};