#pragma once

#include <roboos/wallet/RobotWallet.h>
#include <roboos/config/RoboOSConfig.h>
#include <roboos/marketplace/TaskMarketplace.h>
#include <roboos/fpr/FPRClient.h>
#include <roboos/verification/ZKVerification.h>
#include <roboos/reputation/ReputationClient.h>
#include <roboos/utils/logger.h>
#include <string>
#include <map>
#include <memory>

namespace roboos {

struct RobotCapabilities {
    std::map<std::string, std::string> capabilities;
};

struct RobotOptions {
    std::string robotId;
    std::shared_ptr<RobotWallet> wallet;
    RoboOSConfig config;
    RobotCapabilities capabilities;
};

class BaseRobot {
protected:
    std::string robotId;
    std::shared_ptr<RobotWallet> wallet;
    ConfigManager config;
    RobotCapabilities capabilities;
    std::unique_ptr<FPRClient> fpr;
    std::unique_ptr<TaskMarketplace> marketplace;
    std::unique_ptr<ZKVerification> verification;
    std::unique_ptr<ReputationClient> reputation;
    Logger& logger;
    bool connected = false;

public:
    BaseRobot(const RobotOptions& options);
    virtual ~BaseRobot();
    
    void initialize();
    void connect();
    void disconnect();
    
    RobotCapabilities getCapabilities() const;
    bool hasCapability(const std::string& capability) const;
    
    FPRClient* getFPR() const;
    TaskMarketplace* getMarketplace() const;
    std::shared_ptr<RobotWallet> getWallet() const;
    std::string getRobotId() const;
    
    void destroy();
};

} // namespace roboos

