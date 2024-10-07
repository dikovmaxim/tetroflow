#pragma once

#include <string>

#include "Datatype.hpp"

class Response : public DataType {
public:
    Response(std::string value) : value(value) {}

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
        return "{\"response\":\"" + value + "\"}";
    }
private:
    std::string value;
};

inline std::shared_ptr<DataType> createResponse(std::string value) {
    return std::make_shared<Response>(value);
}