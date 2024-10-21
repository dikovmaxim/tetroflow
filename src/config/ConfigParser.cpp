#include "ConfigParser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

// Constructor implementation
ConfigParser::ConfigParser(const std::string& filename) {
    parseFile(filename);
}

// Check if a key exists
bool ConfigParser::hasKey(const std::string& key) const {
    return configMap.find(key) != configMap.end();
}

// Get value as string
std::string ConfigParser::getString(const std::string& key) const {
    auto it = configMap.find(key);
    if (it != configMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Key not found: " + key);
}

// Get value as integer
int ConfigParser::getInt(const std::string& key) const {
    std::string value = getString(key);
    try {
        return std::stoi(value);
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Invalid integer for key: " + key);
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Integer out of range for key: " + key);
    }
}

// Get value as list of strings
std::vector<std::string> ConfigParser::getList(const std::string& key) const {
    std::string value = getString(key);
    std::vector<std::string> list;
    std::stringstream ss(value);
    std::string item;

    while (std::getline(ss, item, ',')) {
        list.emplace_back(trim(item));
    }

    return list;
}

// Helper method to trim whitespace
std::string ConfigParser::trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }

    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }

    return s.substr(start, end - start);
}

// Parse the configuration file
void ConfigParser::parseFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Unable to open config file: " + filename);
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(infile, line)) {
        ++lineNumber;
        // Trim the line
        std::string trimmed = trim(line);

        // Ignore empty lines and comments
        if (trimmed.empty() || trimmed[0] == ';') {
            continue;
        }

        // Find the delimiter '='
        size_t delimiterPos = trimmed.find('=');
        if (delimiterPos == std::string::npos) {
            // Invalid line format
            throw std::runtime_error("Invalid line format at line " + std::to_string(lineNumber));
        }

        // Extract key and value
        std::string key = trim(trimmed.substr(0, delimiterPos));
        std::string value = trim(trimmed.substr(delimiterPos + 1));

        if (key.empty()) {
            throw std::runtime_error("Empty key at line " + std::to_string(lineNumber));
        }

        // Insert into the map (overwrite if key already exists)
        configMap[key] = value;
    }

    infile.close();
}
