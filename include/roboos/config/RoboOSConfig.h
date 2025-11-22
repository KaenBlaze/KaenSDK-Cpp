#pragma once

#include <string>
#include <memory>

namespace roboos {

enum class Network {
    MAINNET_BETA,
    TESTNET,
    DEVNET,
    LOCALNET
};

struct RoboOSConfig {
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
    
    RoboOSConfig() = default;
    RoboOSConfig(const RoboOSConfig&) = default;
    RoboOSConfig& operator=(const RoboOSConfig&) = default;
};

class ConfigManager {
private:
    RoboOSConfig config;
    
    RoboOSConfig mergeWithDefaults(const RoboOSConfig& partial);
    std::string getDefaultFPREndpoint(Network network) const;
    std::string getDefaultMarketplaceEndpoint(Network network) const;
    std::string getDefaultReputationEndpoint(Network network) const;
    std::string getDefaultVerificationEndpoint(Network network) const;
    std::string getDefaultRPCEndpoint(Network network) const;
    void validateConfig() const;

public:
    ConfigManager(const RoboOSConfig& config = RoboOSConfig());
    
    RoboOSConfig getConfig() const;
    void updateConfig(const RoboOSConfig& updates);
};

} // namespace roboos

