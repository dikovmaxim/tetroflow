#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>

// Command structure to store parsed details
struct Command {
    std::string command;            // Command name (e.g., LSET, GET, etc.)
    std::vector<int> indexChain;    // Chain of indices (e.g., [0, 0, 1])
    std::vector<std::string> types; // Vector to store parameter types
    std::vector<std::string> values; // Vector to store parameter values
};

// Parser class to handle parsing
class CommandParser {
public:
    CommandParser(const std::string& input) : inputString(input) {
        parseCommand();
    }

    void parseCommand() {
        // Regular expression to match the command, database, key indices chain, and multiple parameters
        std::regex commandRegex(R"((\w+)\s+db(\[\d+\](?:\.(?:key|get)\[\d+\])*)\s*(.*))");
        std::smatch matches;

        if (std::regex_search(inputString, matches, commandRegex)) {
            // Extract command type (e.g., LSET, GET, SET)
            command.command = matches[1].str();

            // Extract the entire chain of indices and process them
            std::string indexChainStr = matches[2].str();
            extractIndexChain(indexChainStr);

            // Extract the parameters (values and types)
            std::string parametersStr = matches[3].str();
            extractParameters(parametersStr);
        }
    }

    // Function to extract the chain of indices from a string like db[0].key[0].get[1]
    void extractIndexChain(const std::string& indexChainStr) {
        std::regex indexRegex(R"(\[(\d+)\])");
        std::smatch match;
        std::string::const_iterator searchStart(indexChainStr.cbegin());

        while (std::regex_search(searchStart, indexChainStr.cend(), match, indexRegex)) {
            // Convert the captured string number to an integer and store in the index chain
            command.indexChain.push_back(std::stoi(match[1].str()));
            searchStart = match.suffix().first;
        }
    }

    // Function to extract multiple parameters and their types
    void extractParameters(const std::string& parametersStr) {
        std::regex paramRegex(R"((\"[^\"]*\"|\d+\.?\d*|\{\{}}|\{}|\[\]))");
        std::smatch match;
        std::string::const_iterator searchStart(parametersStr.cbegin());

        while (std::regex_search(searchStart, parametersStr.cend(), match, paramRegex)) {
            std::string value = match[1].str();
            

            // Determine type of the parameter based on the value format
            if (value == "[]") {
                command.types.push_back("LIST");
            } else if (value == "{}") {
                command.types.push_back("SET");
            } else if (value == "{{}}") {
                command.types.push_back("SORTED_SET");
            } else if (value.front() == '\"' && value.back() == '\"') {
                command.types.push_back("STRING");
                value = value.substr(1, value.size() - 2);
            } else if (value.find('.') != std::string::npos) {
                command.types.push_back("FLOAT");
            } else {
                command.types.push_back("INTEGER");
            }

            command.values.push_back(value);

            searchStart = match.suffix().first;
        }
    }

    void printCommand() {
        std::cout << "Command: " << command.command << "\n";
        std::cout << "Index Chain: ";
        for (int index : command.indexChain) {
            std::cout << index << " ";
        }
        std::cout << "\n";

        std::cout << "Parameters:\n";
        for (size_t i = 0; i < command.values.size(); ++i) {
            std::cout << "  Type: " << command.types[i] << " | Value: " << command.values[i] << "\n";
        }
    }

    Command getCommand() {
        return command;
    }

private:
    std::string inputString;
    Command command;
};