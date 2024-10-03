
#include <iostream>
#include <ctime>
#include <string>

#include "Logger.hpp"

std::string getTimestamp() { //in format "YYYY-MM-DD HH:MM:SS"
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return buf;
}

void log(int level, const std::string& message) {
    std::string levelStr;
    switch (level) {
        case LOG_DEBUG:
            levelStr = "DEBUG";
            break;
        case LOG_INFO:
            levelStr = "INFO";
            break;
        case LOG_WARNING:
            levelStr = "WARNING";
            break;
        case LOG_ERROR:
            levelStr = "ERROR";
            break;
        default:
            levelStr = "UNKNOWN";
    }
    std::cout << getTimestamp() << " [" << levelStr << "] " << message << std::endl;
}