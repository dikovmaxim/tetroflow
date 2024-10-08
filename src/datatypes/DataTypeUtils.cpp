#include "Datatype.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <string>
#include <sstream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <cmath>

#include "Integer.hpp"
#include "String.hpp"
#include "Float.hpp"
#include "List.hpp"
#include "Set.hpp"
#include "Sortedset.hpp"

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string DataTypeType_to_string(DataTypeType type) {
    switch (type) {
        case DataTypeType::STRING:
            return "STRING";
        case DataTypeType::INTEGER:
            return "INTEGER";
        case DataTypeType::FLOAT:
            return "FLOAT";
        case DataTypeType::BITMAP:
            return "BITMAP";
        case DataTypeType::LIST:
            return "LIST";
        case DataTypeType::SET:
            return "SET";
        case DataTypeType::SORTEDSET:
            return "SORTEDSET";
        case DataTypeType::HASH:
            return "HASH";
        case DataTypeType::GEO:
            return "GEO";
        case DataTypeType::ERROR:
            return "ERROR";
        case DataTypeType::BOOLEAN:
            return "BOOLEAN";
    }
    return "UNKNOWN";
}

DataTypeType stringToDataTypeType(std::string type) {
    if(type == "STRING") return DataTypeType::STRING;
    if(type == "INTEGER") return DataTypeType::INTEGER;
    if(type == "FLOAT") return DataTypeType::FLOAT;
    if(type == "BITMAP") return DataTypeType::BITMAP;
    if(type == "LIST") return DataTypeType::LIST;
    if(type == "SET") return DataTypeType::SET;
    if(type == "SORTEDSET") return DataTypeType::SORTEDSET;
    if(type == "BOOLEAN") return DataTypeType::BOOLEAN;
    if(type == "STRING") return DataTypeType::STRING;

    return DataTypeType::UNDEFINED;
}

std::string bytesToString(const std::vector<std::byte>& bytes) {
    //return a base64 encoded string
    return base64_encode(bytes);
}

std::string base64_encode(const std::vector<std::byte>& bytes) {
    std::string encoded_string;
    int val = 0;
    int valb = -6;
    
    for (std::byte byte : bytes) {
        val = (val << 8) + static_cast<unsigned char>(byte);
        valb += 8;
        while (valb >= 0) {
            encoded_string.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) {
        encoded_string.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    while (encoded_string.size() % 4) {
        encoded_string.push_back('=');
    }
    
    return encoded_string;
}

std::shared_ptr<DataType> from_string(std::string str, std::string type) {
    std::transform(type.begin(), type.end(), type.begin(), ::toupper);
    if (type == "STRING") {
        return std::make_shared<String>(str);
    } else if (type == "INTEGER") {
        return std::make_shared<Integer>(std::stoi(str));
    } else if (type == "FLOAT") {
        return std::make_shared<Float>(std::stof(str));
    } else if (type == "LIST") {
        return std::make_shared<List>();
    } else if (type == "SET") {
        return std::make_shared<Set>();
    } else if (type == "SORTEDSET") {
        return std::make_shared<Sortedset>();
    } else {
        throw std::invalid_argument("Unknown data type");
    }
}