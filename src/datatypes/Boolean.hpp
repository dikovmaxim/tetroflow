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

private:
    bool value;
};

inline std::shared_ptr<DataType> createBoolean(bool value) {
    return std::make_shared<Boolean>(value);
}