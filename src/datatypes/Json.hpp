#pragma once

#include <string>

#include "Datatype.hpp"
#include "../server/json_fwd.hpp"

class Json : public DataType {
public:
    Json(nlohmann::json value) : value(value) {}

    nlohmann::json get_value() const {
        return value;
    }

    std::shared_ptr<DataType> copy() const override {
        return std::make_shared<Json>(value);
    }

    void set_value(std::string value) {
        this->value = value;
    }

    DataTypeType get_type() const override {
        return DataTypeType::JSON;
    }

    std::string to_string() const override {
        return value.dump();
    }

    bool operator==(const DataType& other) const override {
        if (get_type() != other.get_type()) {
            return false;
        }
        return value == static_cast<const Json&>(other).get_value();
    }

private:
    nlohmann::json value;
};

inline std::shared_ptr<DataType> createJson(nlohmann::json value) {
    return std::make_shared<Json>(value);
}