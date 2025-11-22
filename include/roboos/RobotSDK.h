#pragma once

#include <roboos/config/RoboOSConfig.h>
#include <roboos/utils/logger.h>
#include <memory>
#include <string>

namespace roboos {

// Forward declarations
class RobotWallet;

struct SDKOptions {
    Network network = Network::MAINNET_BETA;
    std::string fprEndpoint;
    std::string marketplaceEndpoint;
    std::string reputationEndpoint;
    std::string verificationEndpoint;
    std::string rpcEndpoint;
    int timeout = 30000;
    int retryAttempts = 3;
    int retryDelay = 1000;
    bool enableLogging = true;
    std::string logLevel = "info";
};

class RobotSDK {
private:
    ConfigManager config;
    Logger& logger;

public:
    RobotSDK(const SDKOptions& options = SDKOptions());
    
    std::unique_ptr<RobotWallet> createWallet(const std::string& path, bool encrypted = true);
    RoboOSConfig getConfig() const;
    void updateConfig(const RoboOSConfig& updates);
    std::string createConnection(const std::string& endpoint = "");
};

} // namespace roboos

