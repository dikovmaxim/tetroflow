#pragma once

#include <string>

#include "Datatype.hpp"

class Error : public DataType {
public:
    Error(std::string value) : value(value) {}

    std::string get_value() const {
        return value;
    }

    void set_value(std::string value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::STRING;
    }

    std::string to_string() const override {
        return "{\"error\": \"" + value + "\"}";
    }

    std::shared_ptr<DataType> copy() const override {
        return std::make_shared<Error>(value);
    }

    bool operator==(const DataType& other) const override {
        if (get_type() != other.get_type()) {
            return false;
        }
        return value == static_cast<const Error&>(other).value;
    }


private:
    std::string value;
};

inline std::shared_ptr<DataType> make_error(std::string value) {
    return std::make_shared<Error>(value);
}