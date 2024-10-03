#pragma once

#include <string>

enum class DataTypeType {
    STRING = 0,
    INTEGER = 1,
    FLOAT = 2,
    BOOLEAN = 3,
    DATE = 4,
    TIME = 5,
    DATETIME = 6,
    BITMAP = 7,
    LIST = 8,
    SET = 9,
    SORTED_SET = 10,
    HASH = 11,
    GEO = 12,
};

class DataType {

public:

    std::string to_string() const;

    virtual DataTypeType get_type() const = 0;

    virtual ~DataType() = default;

};