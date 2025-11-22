#include <roboos/robots/BaseRobot.h>
#include <roboos/fpr/FPRClient.h>
#include <roboos/marketplace/TaskMarketplace.h>
#include <roboos/verification/ZKVerification.h>
#include <roboos/reputation/ReputationClient.h>

namespace roboos {

BaseRobot::BaseRobot(const RobotOptions& options)
    : robotId(options.robotId),
      wallet(options.wallet),
      config(ConfigManager(options.config)),
      capabilities(options.capabilities),
      logger(getLogger()) {
}

BaseRobot::~BaseRobot() {
    destroy();
}

void BaseRobot::initialize() {
    RoboOSConfig cfg = config.getConfig();
    
    // Initialize FPR client
    FPRConfig fprConfig;
    fprConfig.endpoint = cfg.fprEndpoint;
    fprConfig.timeout = cfg.timeout;
    fprConfig.retryAttempts = cfg.retryAttempts;
    fprConfig.retryDelay = cfg.retryDelay;
    fpr = std::make_unique<FPRClient>(fprConfig);
    
    // Initialize marketplace
    MarketplaceConfig marketplaceConfig;
    marketplaceConfig.endpoint = cfg.marketplaceEndpoint;
    marketplaceConfig.timeout = cfg.timeout;
    marketplaceConfig.retryAttempts = cfg.retryAttempts;
    marketplaceConfig.retryDelay = cfg.retryDelay;
    marketplace = std::make_unique<TaskMarketplace>(marketplaceConfig);
    marketplace->setRobotId(robotId);
    
    // Initialize verification
    VerificationConfig verificationConfig;
    verificationConfig.endpoint = cfg.verificationEndpoint;
    verificationConfig.timeout = cfg.timeout;
    verification = std::make_unique<ZKVerification>(verificationConfig);
    
    // Initialize reputation
    if (!cfg.reputationEndpoint.empty()) {
        ReputationConfig reputationConfig;
        reputationConfig.endpoint = cfg.reputationEndpoint;
        reputationConfig.timeout = cfg.timeout;
        reputationConfig.retryAttempts = cfg.retryAttempts;
        reputationConfig.retryDelay = cfg.retryDelay;
        reputation = std::make_unique<ReputationClient>(reputationConfig);
    }
    
    std::map<std::string, std::string> metadata;
    metadata["robotId"] = robotId;
    logger.info("Robot initialized", metadata);
}

void BaseRobot::connect() {
    if (connected) {
        return;
    }
    
    if (!fpr) {
        throw RoboOSError(ErrorCode::INVALID_CONFIG, "Robot not initialized. Call initialize() first.");
    }
    
    std::string publicKey = wallet->getPublicKey();
    fpr->connect(robotId, publicKey);
    
    connected = true;
    logger.info("Robot connected to RoboOS", {{"robotId", robotId}});
}

void BaseRobot::disconnect() {
    if (!connected) {
        return;
    }
    
    if (fpr) {
        fpr->disconnect();
    }
    
    connected = false;
    logger.info("Robot disconnected from RoboOS", {{"robotId", robotId}});
}

RobotCapabilities BaseRobot::getCapabilities() const {
    return capabilities;
}

bool BaseRobot::hasCapability(const std::string& capability) const {
    return capabilities.capabilities.find(capability) != capabilities.capabilities.end();
}

FPRClient* BaseRobot::getFPR() const {
    return fpr.get();
}

TaskMarketplace* BaseRobot::getMarketplace() const {
    return marketplace.get();
}

std::shared_ptr<RobotWallet> BaseRobot::getWallet() const {
    return wallet;
}

std::string BaseRobot::getRobotId() const {
    return robotId;
}

void BaseRobot::destroy() {
    disconnect();
    fpr.reset();
    marketplace.reset();
    verification.reset();
    reputation.reset();
}

} // namespace roboos

