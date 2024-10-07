#include "logger.hpp"

Logger globalLogger;

Logger::Logger() {
    std::cout << "New instance of logger is initialized" << std::endl;
    this->logs = std::vector<std::string>();
}

std::vector<std::string> Logger::getLogs() {
    return this->logs;
}