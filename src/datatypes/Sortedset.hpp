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
        return DataTypeType::SORTEDSET;
    }

    std::string to_string() const override {
        std::string str = "[";
        for (auto it = value.begin(); it != value.end(); it++) {
            str += (*it)->to_string();
            if (it != value.end()) {
                str += ",";
            }
        }
        if(value.size() > 0) str.pop_back();
        str += "]";
        return str;
    }

    std::shared_ptr<DataType> copy() const override {
        return std::make_shared<Sortedset>(value);
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

    bool operator==(const DataType& other) const override {
        if (get_type() != other.get_type()) {
            return false;
        }
        Sortedset otherSortedset = static_cast<const Sortedset&>(other);
        return value == otherSortedset.get_value();
    }

private:
    std::set<std::shared_ptr<DataType>> value;
};