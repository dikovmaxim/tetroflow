#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>

class ConfigParser {
public:
    // Constructor that takes the path to the config file
    ConfigParser(const std::string& filename);

    // Checks if a key exists in the configuration
    bool hasKey(const std::string& key) const;

    // Retrieves the value associated with a key as a string
    // Throws std::runtime_error if the key does not exist
    std::string getString(const std::string& key) const;

    // Retrieves the value associated with a key as an integer
    // Throws std::runtime_error if the key does not exist or conversion fails
    int getInt(const std::string& key) const;

    // Retrieves the value associated with a key as a list of strings
    // Splits the value by commas and trims whitespace
    // Throws std::runtime_error if the key does not exist
    std::vector<std::string> getList(const std::string& key) const;

private:
    std::unordered_map<std::string, std::string> configMap;

    // Helper method to trim whitespace from both ends of a string
    static std::string trim(const std::string& s);

    // Parses the configuration file and populates configMap
    void parseFile(const std::string& filename);
};

#endif // CONFIGPARSER_HPP
