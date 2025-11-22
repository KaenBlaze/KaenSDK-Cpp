#include <roboos/config/RoboOSConfig.h>
#include <roboos/utils/errors.h>
#include <stdexcept>

namespace roboos {

std::string ConfigManager::getDefaultFPREndpoint(Network network) const {
    switch (network) {
        case Network::MAINNET_BETA:
            return "https://fpr.theroboos.com";
        case Network::TESTNET:
            return "https://fpr-testnet.theroboos.com";
        case Network::DEVNET:
            return "https://fpr-devnet.theroboos.com";
        case Network::LOCALNET:
            return "http://localhost:3001";
    }
    return "https://fpr.theroboos.com";
}

std::string ConfigManager::getDefaultMarketplaceEndpoint(Network network) const {
    switch (network) {
        case Network::MAINNET_BETA:
            return "https://marketplace.theroboos.com";
        case Network::TESTNET:
            return "https://marketplace-testnet.theroboos.com";
        case Network::DEVNET:
            return "https://marketplace-devnet.theroboos.com";
        case Network::LOCALNET:
            return "http://localhost:3002";
    }
    return "https://marketplace.theroboos.com";
}

std::string ConfigManager::getDefaultReputationEndpoint(Network network) const {
    switch (network) {
        case Network::MAINNET_BETA:
            return "https://reputation.theroboos.com";
        case Network::TESTNET:
            return "https://reputation-testnet.theroboos.com";
        case Network::DEVNET:
            return "https://reputation-devnet.theroboos.com";
        case Network::LOCALNET:
            return "http://localhost:3003";
    }
    return "https://reputation.theroboos.com";
}

std::string ConfigManager::getDefaultVerificationEndpoint(Network network) const {
    switch (network) {
        case Network::MAINNET_BETA:
            return "https://verification.theroboos.com";
        case Network::TESTNET:
            return "https://verification-testnet.theroboos.com";
        case Network::DEVNET:
            return "https://verification-devnet.theroboos.com";
        case Network::LOCALNET:
            return "http://localhost:3004";
    }
    return "https://verification.theroboos.com";
}

std::string ConfigManager::getDefaultRPCEndpoint(Network network) const {
    switch (network) {
        case Network::MAINNET_BETA:
            return "https://api.mainnet-beta.solana.com";
        case Network::TESTNET:
            return "https://api.testnet.solana.com";
        case Network::DEVNET:
            return "https://api.devnet.solana.com";
        case Network::LOCALNET:
            return "http://localhost:8899";
    }
    return "https://api.mainnet-beta.solana.com";
}

RoboOSConfig ConfigManager::mergeWithDefaults(const RoboOSConfig& partial) {
    RoboOSConfig merged = partial;
    
    if (merged.fprEndpoint.empty()) {
        merged.fprEndpoint = getDefaultFPREndpoint(merged.network);
    }
    if (merged.marketplaceEndpoint.empty()) {
        merged.marketplaceEndpoint = getDefaultMarketplaceEndpoint(merged.network);
    }
    if (merged.reputationEndpoint.empty()) {
        merged.reputationEndpoint = getDefaultReputationEndpoint(merged.network);
    }
    if (merged.verificationEndpoint.empty()) {
        merged.verificationEndpoint = getDefaultVerificationEndpoint(merged.network);
    }
    if (merged.rpcEndpoint.empty()) {
        merged.rpcEndpoint = getDefaultRPCEndpoint(merged.network);
    }
    
    return merged;
}

void ConfigManager::validateConfig() const {
    if (config.fprEndpoint.empty()) {
        throw RoboOSError(ErrorCode::INVALID_CONFIG, "FPR endpoint is required");
    }
    if (config.marketplaceEndpoint.empty()) {
        throw RoboOSError(ErrorCode::INVALID_CONFIG, "Marketplace endpoint is required");
    }
    if (config.timeout < 0) {
        throw RoboOSError(ErrorCode::INVALID_CONFIG, "Timeout must be non-negative");
    }
    if (config.retryAttempts < 0) {
        throw RoboOSError(ErrorCode::INVALID_CONFIG, "Retry attempts must be non-negative");
    }
}

ConfigManager::ConfigManager(const RoboOSConfig& config) {
    this->config = mergeWithDefaults(config);
    validateConfig();
}

RoboOSConfig ConfigManager::getConfig() const {
    return config;
}

void ConfigManager::updateConfig(const RoboOSConfig& updates) {
    config = mergeWithDefaults(updates);
    validateConfig();
}

} // namespace roboos

