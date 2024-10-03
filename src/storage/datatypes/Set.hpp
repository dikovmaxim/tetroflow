#pragma once

#include <string>
#include <unordered_set>
#include <memory>
#include <vector>

#include "Datatype.hpp"

class Set : public DataType {
public:
    Set(std::unordered_set<std::shared_ptr<DataType>> value) : value(value) {}

    std::unordered_set<std::shared_ptr<DataType>> get_value() const {
        return value;
    }

    void set_value(std::unordered_set<std::shared_ptr<DataType>> value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::SET;
    }

    std::string to_string() const override {
        std::string str = "{";
        for (auto it = value.begin(); it != value.end(); it++) {
            str += (*it)->to_string();
            if (it != value.end()) {
                str += ", ";
            }
        }
        str += "}";
        return str;
    }

    void sadd(std::shared_ptr<DataType> value) {
        this->value.insert(value);
    }

    void srem(std::shared_ptr<DataType> value) {
        this->value.erase(value);
    }

    bool sismember(std::shared_ptr<DataType> value) {
        return this->value.find(value) != this->value.end();
    }

    std::vector<std::shared_ptr<DataType>> smembers() {
        std::vector<std::shared_ptr<DataType>> members;
        for (auto it = value.begin(); it != value.end(); it++) {
            std::shared_ptr<DataType> member = *it;
            members.push_back(member);
        }
        return members;
    }

private:
    std::unordered_set<std::shared_ptr<DataType>> value;
};
