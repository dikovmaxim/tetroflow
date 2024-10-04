#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>



#include "Datatype.hpp"
#include "Iterable.hpp"

class List : public DataType , public Iterable {
public:
    List(std::list<std::shared_ptr<DataType>> value) : value(value) {}
    //empty list constructor to just initialize the list
    List() : value(std::list<std::shared_ptr<DataType>>()) {}

    std::list<std::shared_ptr<DataType>> get_value() const {
        return value;
    }

    void set_value(std::list<std::shared_ptr<DataType>> value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::LIST;
    }

    std::shared_ptr<DataType> get(int index) override {
        if (index < 0 || index >= value.size()) {
            throw std::invalid_argument("Index out of bounds");
        }
        auto it = value.begin();
        std::advance(it, index);
        return *it;
    }

    void set(int index, std::shared_ptr<DataType> value) override {
        if (index < 0 || index >= this->value.size()) {
            throw std::invalid_argument("Index out of bounds");
        }
        std::shared_ptr<DataType> newValue = value;
        auto it = this->value.begin();
        std::advance(it, index);
        *it = newValue;
    }

    int size() override {
        return value.size();
    }

    std::string to_string() const override {
        std::string str = "[";
        for (auto it = value.begin(); it != value.end(); it++) {
            str += (*it)->to_string();
            if (it != value.end()) {
                str += ", ";
            }
        }
        str += "]";
        return str;
    }

    void lpush(std::shared_ptr<DataType> value) {
        this->value.push_front(value);
    }

    void rpush(std::shared_ptr<DataType> value) {
        this->value.push_back(value);
    }

    std::shared_ptr<DataType> lpop() {
        if (value.empty()) {
            return nullptr;
        }
        std::shared_ptr<DataType> value = this->value.front();
        this->value.pop_front();
        return value;
    }

    std::shared_ptr<DataType> rpop() {
        if (value.empty()) {
            return nullptr;
        }
        std::shared_ptr<DataType> value = this->value.back();
        this->value.pop_back();
        return value;
    }

    std::shared_ptr<DataType> lindex(uint32_t index) {
        if (index >= value.size()) {
            return nullptr;
        }
        auto it = value.begin();
        std::advance(it, index);
        return *it;
    }

    uint32_t llen() {
        return value.size();
    }

    void lset(uint32_t index, std::shared_ptr<DataType> value) {
        if (index >= this->value.size()) {
            return;
        }
        auto it = this->value.begin();
        std::advance(it, index);
        *it = value;
    }

    void linsert(uint32_t index, std::shared_ptr<DataType> value) {
        if (index >= this->value.size()) {
            return;
        }
        auto it = this->value.begin();
        std::advance(it, index);
        this->value.insert(it, value);
    }

    void lrem(uint32_t count, std::shared_ptr<DataType> value) {
        auto it = this->value.begin();
        while (it != this->value.end()) {
            if (*it == value) {
                it = this->value.erase(it);
                count--;
                if (count == 0) {
                    break;
                }
            } else {
                it++;
            }
        }
    }

    ~List(){
        value.clear();
    }


private:
    std::list<std::shared_ptr<DataType>> value;
};

inline std::shared_ptr<DataType> createList(std::list<std::shared_ptr<DataType>> value) {
    return std::make_shared<List>(value);
}

inline std::shared_ptr<DataType> createEmptyList() {
    return std::make_shared<List>();
}