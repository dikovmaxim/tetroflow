#pragma once

#include <string>

#include "Datatype.hpp"

class Float : public DataType {
public:
    Float(float value) : value(value) {}

    float get_value() const {
        return value;
    }

    void set_value(float value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::FLOAT;
    }

    std::string to_string() const override {
        return std::to_string(value);
    }

    std::shared_ptr<DataType> copy() const override {
        return std::make_shared<Float>(value);
    }

    bool operator==(const DataType& other) const override {
        if (get_type() != other.get_type()) {
            return false;
        }
        return value == static_cast<const Float&>(other).value;
    }

private:
    float value;
};

inline std::shared_ptr<DataType> createFloat(float value) {
    return std::make_shared<Float>(value);
}