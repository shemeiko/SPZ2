#pragma once
#include <string>
#include <fstream>
#include <mutex>

enum class LogLevel : uint8_t {
    ALL = 1, INFO, DEBUG, WARNING, ERROR
};

class Logger {
private:
    static std::mutex mutex;
    std::ofstream outputFile;
    std::string format = "%H:%M:%S";
    LogLevel lowBoundLevel = LogLevel::ALL;

    Logger();
    ~Logger();
    std::string strFormat(const std::string&, const std::string&);
public:
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;

    static Logger& getInstance();
    void setTimestampFormat(const std::string&);
    void setLevel(LogLevel);
    void openLogFile(const std::string&);
    void log(LogLevel, const std::string&);
};