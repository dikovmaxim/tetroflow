#pragma once

#include <string>
#include <unordered_set>
#include <memory>
#include <vector>

#include "Datatype.hpp"
#include "interfaces/Iterable.hpp"

class Set : public DataType{
public:

    //Constructors

    Set(std::unordered_set<std::shared_ptr<DataType>, DataTypePtrHash, DataTypePtrEqual> value) : value(value) {}
    Set() : value(std::unordered_set<std::shared_ptr<DataType>, DataTypePtrHash, DataTypePtrEqual>()) {}

    std::unordered_set<std::shared_ptr<DataType>, DataTypePtrHash, DataTypePtrEqual> get_value() const {
        return value;
    }

    void set_value(std::unordered_set<std::shared_ptr<DataType>, DataTypePtrHash, DataTypePtrEqual> value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::SET;
    }

    std::string to_string() const override {
        std::string str = "[";
        for (auto it = value.begin(); it != value.end(); it++) {
            str += (*it)->to_string();
            if (it != value.end()) {
                str += ",";
            }
        }
        // remove last comma
        if (str.length() > 1) str.pop_back();
        str += "]";
        return str;
    }

    std::shared_ptr<DataType> copy() const override {
        return std::make_shared<Set>(value);
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

    int scard() {
        return value.size();
    }

    ~Set(){
        value.clear();
    }

    bool operator==(const DataType& other) const override {
        if (get_type() != other.get_type()) {
            return false;
        }
        Set otherSet = static_cast<const Set&>(other);
        return value == otherSet.get_value();
    }



private:
    std::unordered_set<std::shared_ptr<DataType>, DataTypePtrHash, DataTypePtrEqual> value;
};

//inline function with ... parameters
inline std::shared_ptr<Set> createSet(std::vector<std::shared_ptr<DataType>> values) {
    std::unordered_set<std::shared_ptr<DataType>, DataTypePtrHash, DataTypePtrEqual> set;
    for (auto it = values.begin(); it != values.end(); it++) {
        set.insert(*it);
    }
    return std::make_shared<Set>(set);
}