#include "Logger.hpp"
#include <mutex>
#include <iostream>
#include <fstream>
#include <format>
#include <string>
#include <stdexcept>

std::mutex Logger::mutex;

Logger::Logger() {}

Logger::~Logger() {
    if (outputFile.is_open())
        outputFile.close();
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

std::string Logger::logFormat(const std::string& level, const std::string& message) {
    if (format.length() == 0)
        throw std::length_error("use method setTimestampFormat to set format");

    time_t raw_time = std::time(nullptr);
    char timeString[100];
    strftime(std::data(timeString), sizeof(timeString), format.c_str(), std::localtime(&raw_time));
    return std::format("{}: {} [{}]: {}", level, timeString, "Logger", message);
}

void Logger::setTimestampFormat(const std::string& format) {
    this->format = format;
}

void Logger::openLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> guard(mutex);

    outputFile.open(filename, std::ios::out | std::ios::app);
    if (!outputFile.is_open()))
        throw std::runtime_error("file doesn't open");
}

void Logger::setLevel(LogLevel level) {
    lowBoundLevel = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lg(mutex);

    if (level < lowBoundLevel) return;

    std::string levelStr;
    switch (level) {
        case LogLevel::INFO: levelStr = "INFO"; break;
        case LogLevel::DEBUG: levelStr = "DEBUG"; break;
        case LogLevel::WARNING: levelStr = "WARNING"; break;
        case LogLevel::ERROR: levelStr = "ERROR"; break;
    }

    if (outputFile.is_open())
        outputFile << logFormat(levelStr, message) << '\n';
}