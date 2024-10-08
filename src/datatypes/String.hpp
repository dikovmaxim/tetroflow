#pragma once

#include <string>

#include "Datatype.hpp"

class String : public DataType {
public:
    String(std::string value) : value(value) {}

    std::string get_value() const {
        return value;
    }

    std::shared_ptr<DataType> copy() const override {
        return std::make_shared<String>(value);
    }

    void set_value(std::string value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::STRING;
    }

    std::string to_string() const override {
        return "\"" + value + "\"";
    }

    void append(std::string value) {
        this->value += value;
    }

private:
    std::string value;
};

inline std::shared_ptr<DataType> createString(std::string value) {
    return std::make_shared<String>(value);
}