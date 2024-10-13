#ifndef DATATYPE_HPP
#define DATATYPE_HPP

#include <string>
#include <vector>
#include <memory>

enum class DataTypeType {
    STRING = 0,
    INTEGER = 1,
    FLOAT = 2,
    BITMAP = 7,
    LIST = 8,
    SET = 9,
    SORTEDSET = 10,
    HASH = 11,
    GEO = 12,
    BOOLEAN = 14,
    UNDEFINED = 15,
    ERROR = 16
};

std::string DataTypeType_to_string(DataTypeType type);
std::string bytesToString(const std::vector<std::byte>& bytes);
std::string base64_encode(const std::vector<std::byte>& bytes);
DataTypeType stringToDataTypeType(std::string type);


class DataType {
public:
    // Make this a virtual method and pure virtual (= 0)
    virtual std::string to_string() const = 0;

    virtual DataTypeType get_type() const = 0;

    virtual ~DataType() = default;

    virtual std::shared_ptr<DataType> copy() const = 0;

    virtual bool operator==(const DataType& other) const = 0;
};

std::shared_ptr<DataType> from_string(std::string str, std::string type);


struct DataTypePtrHash {
    std::size_t operator()(const std::shared_ptr<DataType>& ptr) const {
        return std::hash<int>()(ptr->to_string().length());
    }
};

// Custom equality function for std::shared_ptr<DataType>
struct DataTypePtrEqual {
    bool operator()(const std::shared_ptr<DataType>& lhs, const std::shared_ptr<DataType>& rhs) const {
        return *lhs == *rhs;  // Compare dereferenced DataType objects
    }
};


#endif //DATATYPE_HPP
