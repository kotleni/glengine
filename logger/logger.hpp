#ifndef H_LOGGER
#define H_LOGGER

#include <string>
#include <vector>
#include <iostream>
#include <stdarg.h>
#include <format>

#define LOGGER_LINES_LIMIT 64
#define LOGGER_PREFIX_INFO "[INFO] "
#define LOGGER_PREFIX_WARN "[WARN] "
#define LOGGER_PREFIX_ERROR "[ERROR] "

#define LOG_INFO(...) globalLogger.logInfo(__VA_ARGS__)
#define LOG_WARN(...) globalLogger.logWarn(__VA_ARGS__)
#define LOG_ERROR(...) globalLogger.logError(__VA_ARGS__)

class Logger {
private:
    std::vector<std::string> logs;

    template<class... Args>
    void log(const char *prefix, std::string_view rt_fmt_str, Args&&... args) {
        std::string line = std::vformat(rt_fmt_str, std::make_format_args(args...));
        std::string totalLine = std::string(prefix);
        totalLine.append(line);
    
        if(this == nullptr) return; // Don't save anything if logs is not prepared
        std::cout << totalLine << std::endl;
        this->logs.push_back(totalLine);

        if(this->logs.size() > LOGGER_LINES_LIMIT)
            this->logs.erase(this->logs.begin());
    }

public:
    Logger();

    template<class... Args>
    void logInfo(std::string_view rt_fmt_str, Args&&... args) {
        this->log(LOGGER_PREFIX_INFO, rt_fmt_str, args...);
    }

    template<class... Args>
    void logWarning(std::string_view rt_fmt_str, Args&&... args) {
        this->log(LOGGER_PREFIX_WARN, rt_fmt_str, args...);
    }

    template<class... Args>
    void logError(std::string_view rt_fmt_str, Args&&... args) {
        this->log(LOGGER_PREFIX_ERROR, rt_fmt_str, args...);
    }

    std::vector<std::string> getLogs();
};

extern Logger globalLogger;

#endif