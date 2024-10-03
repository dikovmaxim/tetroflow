#include "Datatype.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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
        case DataTypeType::SORTED_SET:
            return "SORTED_SET";
        case DataTypeType::HASH:
            return "HASH";
        case DataTypeType::GEO:
            return "GEO";
    }
    return "UNKNOWN";
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