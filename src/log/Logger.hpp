#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARNING 2
#define LOG_ERROR 3

void log(int level, const std::string& message);

#endif // LOGGER_HPP