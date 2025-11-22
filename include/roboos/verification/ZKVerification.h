#pragma once

#include <roboos/utils/errors.h>
#include <roboos/utils/logger.h>
#include <string>
#include <map>

namespace roboos {

struct ProofGenerationOptions {
    std::string taskId;
    std::map<std::string, std::string> taskData;
    std::string robotId;
    bool includeLocation = true;
    bool includeTimestamp = true;
};

struct TaskProof {
    std::string proofId;
    std::string taskId;
    std::string robotId;
    std::string proofData;
    std::string timestamp;
};

struct VerificationConfig {
    std::string endpoint;
    int timeout = 30000;
};

class ZKVerification {
private:
    VerificationConfig config;
    Logger& logger;

public:
    ZKVerification(const VerificationConfig& config);
    
    TaskProof generateProof(const ProofGenerationOptions& options);
    void submitProof(const TaskProof& proof);
};

} // namespace roboos

