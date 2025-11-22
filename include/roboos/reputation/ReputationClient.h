#pragma once

#include <roboos/utils/errors.h>
#include <roboos/utils/logger.h>
#include <string>

namespace roboos {

struct ReputationScore {
    std::string robotId;
    double score;
    int completedTasks;
    int totalTasks;
    std::string lastUpdated;
};

struct ReputationUpdate {
    std::string robotId;
    std::string taskId;
    bool success;
    std::string timestamp;
};

struct ReputationConfig {
    std::string endpoint;
    int timeout = 30000;
    int retryAttempts = 3;
    int retryDelay = 1000;
};

class ReputationClient {
private:
    ReputationConfig config;
    Logger& logger;

public:
    ReputationClient(const ReputationConfig& config);
    
    ReputationScore getScore(const std::string& robotId);
    void submitUpdate(const ReputationUpdate& update);
    void destroy();
};

} // namespace roboos

