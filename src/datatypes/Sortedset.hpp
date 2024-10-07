#pragma once

#include <string>
#include <set>
#include <memory>
#include <vector>


#include "Datatype.hpp"

class Sortedset : public DataType {
public:
    Sortedset(std::set<std::shared_ptr<DataType>> value) : value(value) {}
    Sortedset() : value(std::set<std::shared_ptr<DataType>>()) {}

    std::set<std::shared_ptr<DataType>> get_value() const {
        return value;
    }

    void set_value(std::set<std::shared_ptr<DataType>> value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::SORTED_SET;
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

    void zadd(std::shared_ptr<DataType> value, const float score) {
        this->value.insert(value);
    }

    void zrem(std::shared_ptr<DataType> value) {
        this->value.erase(value);
    }

    std::vector<std::shared_ptr<DataType>> zrange(const uint32_t start, const uint32_t end) {
        std::vector<std::shared_ptr<DataType>> result;
        auto it = value.begin();
        for (uint32_t i = 0; i < start; i++) {
            it++;
        }
        for (uint32_t i = start; i < end; i++) {
            result.push_back(*it);
            it++;
        }
        return result;
    }

    std::vector<std::shared_ptr<DataType>> zrevrange(const uint32_t start, const uint32_t end) {
        std::vector<std::shared_ptr<DataType>> result;
        auto it = value.rbegin();
        for (uint32_t i = 0; i < start; i++) {
            it++;
        }
        for (uint32_t i = start; i < end; i++) {
            result.push_back(*it);
            it++;
        }
        return result;
    }

    uint32_t zcard() {
        return value.size();
    }

    ~Sortedset(){
        value.clear();
    }

private:
    std::set<std::shared_ptr<DataType>> value;
};