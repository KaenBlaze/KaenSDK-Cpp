#include <roboos/RobotSDK.h>
#include <roboos/wallet/RobotWallet.h>
#include <roboos/utils/logger.h>

namespace roboos {

RobotSDK::RobotSDK(const SDKOptions& options) : logger(getLogger()) {
    // Set log level
    if (options.logLevel == "debug") {
        setLogLevel(LogLevel::DEBUG);
    } else if (options.logLevel == "info") {
        setLogLevel(LogLevel::INFO);
    } else if (options.logLevel == "warn") {
        setLogLevel(LogLevel::WARN);
    } else if (options.logLevel == "error") {
        setLogLevel(LogLevel::ERROR);
    }
    
    // Initialize config
    RoboOSConfig config;
    config.network = options.network;
    config.fprEndpoint = options.fprEndpoint;
    config.marketplaceEndpoint = options.marketplaceEndpoint;
    config.reputationEndpoint = options.reputationEndpoint;
    config.verificationEndpoint = options.verificationEndpoint;
    config.rpcEndpoint = options.rpcEndpoint;
    config.timeout = options.timeout;
    config.retryAttempts = options.retryAttempts;
    config.retryDelay = options.retryDelay;
    config.enableLogging = options.enableLogging;
    config.logLevel = options.logLevel;
    
    this->config = ConfigManager(config);
    
    std::map<std::string, std::string> metadata;
    metadata["network"] = std::to_string(static_cast<int>(this->config.getConfig().network));
    metadata["fprEndpoint"] = this->config.getConfig().fprEndpoint;
    logger.info("RobotSDK initialized", metadata);
}

std::unique_ptr<RobotWallet> RobotSDK::createWallet(const std::string& path, bool encrypted) {
    WalletOptions options;
    options.storage = StorageType::FILE;
    options.path = path;
    options.encrypted = encrypted;
    return RobotWallet::create(options);
}

RoboOSConfig RobotSDK::getConfig() const {
    return config.getConfig();
}

void RobotSDK::updateConfig(const RoboOSConfig& updates) {
    config.updateConfig(updates);
}

std::string RobotSDK::createConnection(const std::string& endpoint) {
    RoboOSConfig cfg = config.getConfig();
    return endpoint.empty() ? cfg.rpcEndpoint : endpoint;
}

} // namespace roboos

