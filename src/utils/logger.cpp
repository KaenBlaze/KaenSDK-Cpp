#include <roboos/utils/logger.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <mutex>

namespace roboos {

static std::unique_ptr<Logger> g_logger = std::make_unique<DefaultLogger>();
static std::mutex g_logger_mutex;

std::string getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void DefaultLogger::log(LogLevel level, const std::string& message,
                        const std::map<std::string, std::string>& metadata) {
    if (level < minLevel) {
        return;
    }
    
    std::string levelStr;
    switch (level) {
        case LogLevel::DEBUG: levelStr = "DEBUG"; break;
        case LogLevel::INFO: levelStr = "INFO"; break;
        case LogLevel::WARN: levelStr = "WARN"; break;
        case LogLevel::ERROR: levelStr = "ERROR"; break;
    }
    
    std::ostringstream oss;
    oss << "[" << getCurrentTimestamp() << "] [" << levelStr << "] " << message;
    
    if (!metadata.empty()) {
        oss << " {";
        bool first = true;
        for (const auto& [key, value] : metadata) {
            if (!first) oss << ", ";
            oss << key << ": " << value;
            first = false;
        }
        oss << "}";
    }
    
    std::lock_guard<std::mutex> lock(g_logger_mutex);
    if (level >= LogLevel::ERROR) {
        std::cerr << oss.str() << std::endl;
    } else {
        std::cout << oss.str() << std::endl;
    }
}

Logger& getLogger() {
    std::lock_guard<std::mutex> lock(g_logger_mutex);
    return *g_logger;
}

void setLogger(std::unique_ptr<Logger> logger) {
    std::lock_guard<std::mutex> lock(g_logger_mutex);
    g_logger = std::move(logger);
}

void setLogLevel(LogLevel level) {
    auto logger = std::make_unique<DefaultLogger>(level);
    setLogger(std::move(logger));
}

} // namespace roboos

