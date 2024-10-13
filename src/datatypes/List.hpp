#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>


#include "Datatype.hpp"
#include "interfaces/Iterable.hpp"

class List : public DataType , public Iterable {
public:

    //Constructors

    List(std::list<std::shared_ptr<DataType>> value) : value(value) {}
    List() : value(std::list<std::shared_ptr<DataType>>()) {}

    //Datatype interface methods

    DataTypeType get_type() const override {
        return DataTypeType::LIST;
    }

    std::string to_string() const override {
        std::string str = "[";
        for (auto it = value.begin(); it != value.end(); it++) {
            str += (*it)->to_string();
            if (std::next(it) != value.end()) {
                str += ",";
            }
        }
        str += "]";
        return str;
    }

    std::shared_ptr<DataType> copy() const override {
        return std::make_shared<List>(value);
    }

    //Iterable interface methods

    void set(int index, std::shared_ptr<DataType> value) override {
        if (index < 0 || index >= this->value.size()) {
            throw std::invalid_argument("Index out of bounds");
        }
        std::shared_ptr<DataType> newValue = value;
        auto it = this->value.begin();
        std::advance(it, index);
        *it = newValue;
    }

    std::shared_ptr<DataType> get(int index) override {
        if (index < 0 || index >= value.size()) {
            throw std::invalid_argument("Index out of bounds");
        }
        auto it = value.begin();
        std::advance(it, index);
        return *it;
    }

    int size() override {
        return value.size();
    }

    //List methods

    std::list<std::shared_ptr<DataType>> get_value() const {
        return value;
    }

    void set_value(std::list<std::shared_ptr<DataType>> value) {
        this->value = value;
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

    bool operator==(const DataType& other) const override {
        if (get_type() != other.get_type()) {
            return false;
        }
        List otherList = static_cast<const List&>(other);
        if (value.size() != otherList.get_value().size()) {
            return false;
        }
        auto it1 = value.begin();
        auto it2 = otherList.get_value().begin();
        while (it1 != value.end()) {
            if (*it1 != *it2) {
                return false;
            }
            it1++;
            it2++;
        }
        return true;
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