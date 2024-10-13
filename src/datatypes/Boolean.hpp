#pragma once

#include <string>

#include "Datatype.hpp"

class Boolean : public DataType {
public:
    Boolean(bool value) : value(value) {}

    bool get_value() const {
        return value;
    }

    void set_value(bool value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::BOOLEAN;
    }

    std::string to_string() const override {
        return value ? "true" : "false";
    }

    std::shared_ptr<DataType> copy() const override {
        return std::make_shared<Boolean>(value);
    }

    bool flip() {
        value = !value;
        return value;
    }

    bool operator==(const DataType& other) const override {
        if (get_type() != other.get_type()) {
            return false;
        }
        return value == static_cast<const Boolean&>(other).value;
    }

private:
    bool value;
};

inline std::shared_ptr<DataType> createBoolean(bool value) {
    return std::make_shared<Boolean>(value);
}