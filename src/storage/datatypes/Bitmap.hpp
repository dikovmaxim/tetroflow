#pragma once

#include <string>
#include <vector>


#include "Datatype.hpp"

class Bitmap : public DataType {
public:
    Bitmap(std::string value) : value(value) {}

    std::vector<std::byte> get_value() const {
        return value;
    }

    void set_value(std::vector<std::byte> value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::BITMAP;
    }

    std::string to_string() const override {
        return bytesToString(value);
    }

private:
    std::vector<std::byte> value;
};

