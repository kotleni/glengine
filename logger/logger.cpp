#include "logger.hpp"

std::string string_format(std::string fmt, ...) {
    int size=100;
    std::string str;
    va_list ap;

    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf(&str[0], size, fmt.c_str(), ap);
        va_end(ap);
   
        if (n > -1 && n < size) {
            str.resize(n); // Make sure there are no trailing zero char
            return str;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }
}

Logger::Logger() {
    _loggerInstance = this;
    std::cout << "New instance of logger is initialized" << std::endl;
    this->logs = std::vector<std::string>();
}

void Logger::log(const char *prefix, std::string format, ...) {
    std::string line = string_format(format);
    std::string totalLine = std::string(prefix);
    totalLine.append(line);

    
    if(this == nullptr) return; // Don't save anything if logs is not prepared
    std::cout << totalLine << std::endl;
    this->logs.push_back(totalLine);

    if(this->logs.size() > LOGGER_LINES_LIMIT)
        this->logs.erase(this->logs.begin());
}

void Logger::logInfo(std::string format, ...) {
    this->log(LOGGER_PREFIX_INFO, format);
}

void Logger::logWarning(std::string format, ...) {
    this->log(LOGGER_PREFIX_WARN, format);
}

void Logger::logError(std::string format, ...) {
    this->log(LOGGER_PREFIX_ERROR, format);
}

std::vector<std::string> Logger::getLogs() {
    return this->logs;
}