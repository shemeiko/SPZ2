#include <iostream>
#include <format>
#include <filesystem>
#include "Logger.hpp"

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    auto& logger = Logger::getInstance();
    logger.openLogFile("log.txt");
    
    if (argc < 2) {
        logger.log(LogLevel::ERROR, "Filename was not passed");
        std::cerr << "Filename was not passed" << '\n';
        return 1;
    }

    try {
        fs::path filepath(argv[1]);

        if (!fs::exists(filepath)) {
            throw std::runtime_error(std::format("File '{}' doesn't exist", argv[1]));
        }

        uintmax_t size = fs::file_size(filepath);
        logger.log(LogLevel::INFO, std::format("File [{}] has been checked on size ({} bytes)", argv[1], size));

        std::cout << "File " << argv[1] << " (" << size << " bytes)" << '\n';
    }
    catch (const std::runtime_error& err) {
        logger.log(LogLevel::ERROR, err.what());
        std::cerr << err.what() << '\n';
    }

    return 0;
}