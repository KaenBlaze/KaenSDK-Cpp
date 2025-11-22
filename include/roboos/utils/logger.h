#pragma once

#include <string>
#include <functional>
#include <memory>
#include <map>

namespace roboos {

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3
};

struct LogEntry {
    LogLevel level;
    std::string message;
    std::string timestamp;
    std::map<std::string, std::string> metadata;
};

class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(LogLevel level, const std::string& message,
                     const std::map<std::string, std::string>& metadata = {}) = 0;
    virtual void debug(const std::string& message,
                       const std::map<std::string, std::string>& metadata = {}) = 0;
    virtual void info(const std::string& message,
                      const std::map<std::string, std::string>& metadata = {}) = 0;
    virtual void warn(const std::string& message,
                      const std::map<std::string, std::string>& metadata = {}) = 0;
    virtual void error(const std::string& message,
                       const std::map<std::string, std::string>& metadata = {}) = 0;
};

class DefaultLogger : public Logger {
private:
    LogLevel minLevel;

public:
    DefaultLogger(LogLevel level = LogLevel::INFO) : minLevel(level) {}
    
    void log(LogLevel level, const std::string& message,
             const std::map<std::string, std::string>& metadata = {}) override;
    
    void debug(const std::string& message,
               const std::map<std::string, std::string>& metadata = {}) override {
        log(LogLevel::DEBUG, message, metadata);
    }
    
    void info(const std::string& message,
              const std::map<std::string, std::string>& metadata = {}) override {
        log(LogLevel::INFO, message, metadata);
    }
    
    void warn(const std::string& message,
              const std::map<std::string, std::string>& metadata = {}) override {
        log(LogLevel::WARN, message, metadata);
    }
    
    void error(const std::string& message,
               const std::map<std::string, std::string>& metadata = {}) override {
        log(LogLevel::ERROR, message, metadata);
    }
};

// Global logger functions
Logger& getLogger();
void setLogger(std::unique_ptr<Logger> logger);
void setLogLevel(LogLevel level);

} // namespace roboos

