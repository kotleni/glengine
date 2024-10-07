#ifndef H_LOGGER
#define H_LOGGER

#include <string>
#include <vector>
#include <iostream>
#include <stdarg.h>

#define LOGGER_LINES_LIMIT 64
#define LOGGER_PREFIX_INFO "[INFO] "
#define LOGGER_PREFIX_WARN "[WARN] "
#define LOGGER_PREFIX_ERROR "[ERROR] "

class Logger {
private:
    std::vector<std::string> logs;

    void log(const char *prefix, std::string format, ...);
public:
    Logger();

    void logInfo(std::string format, ...);
    void logWarning(std::string format, ...);
    void logError(std::string format, ...);
    std::vector<std::string> getLogs();
};

static Logger *_loggerInstance;
static Logger *logger() {
    if(_loggerInstance == nullptr)
        _loggerInstance = new Logger();
    return _loggerInstance;
}

#endif