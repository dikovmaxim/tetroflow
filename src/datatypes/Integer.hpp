#pragma once

#include "Datatype.hpp"

#include <memory>
#include <vector>
#include <string>



class Integer : public DataType {
public:
    Integer(int value) : value(value) {}

    int get_value() const {
        return value;
    }

    void set_value(int value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::INTEGER;
    }

    std::string to_string() const override {
        return std::to_string(value);
    }

    std::shared_ptr<DataType> copy() const override {
        return std::make_shared<Integer>(value);
    }

    void increment() {
        value++;
    }

    void decrement() {
        value--;
    }

    void incrementby(int value) {
        this->value += value;
    }

    void decrementby(int value) {
        this->value -= value;
    }

    

private:
    int value;
};

inline std::shared_ptr<DataType> createInteger(int value) {
    return std::make_shared<Integer>(value);
}