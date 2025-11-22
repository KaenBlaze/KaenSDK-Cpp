#pragma once

#include <string>
#include <stdexcept>
#include <map>

namespace roboos {

enum class ErrorCode {
    // Wallet errors
    WALLET_NOT_FOUND,
    WALLET_DECRYPTION_FAILED,
    WALLET_ENCRYPTION_FAILED,
    INVALID_KEYPAIR,
    INSUFFICIENT_BALANCE,

    // FPR errors
    FPR_CONNECTION_FAILED,
    FPR_CHANNEL_NOT_FOUND,
    FPR_CHANNEL_CLOSED,
    FPR_INSUFFICIENT_COLLATERAL,
    FPR_PAYMENT_FAILED,
    FPR_TIMEOUT,

    // Marketplace errors
    TASK_NOT_FOUND,
    BID_REJECTED,
    TASK_ALREADY_ASSIGNED,
    INVALID_BID,

    // Verification errors
    PROOF_GENERATION_FAILED,
    VERIFICATION_FAILED,
    INVALID_PROOF,

    // Reputation errors
    REPUTATION_QUERY_FAILED,
    REPUTATION_UPDATE_FAILED,

    // General errors
    NETWORK_ERROR,
    INVALID_CONFIG,
    OPERATION_TIMEOUT,
    RATE_LIMIT_EXCEEDED
};

class RoboOSError : public std::runtime_error {
public:
    ErrorCode code;
    std::map<std::string, std::string> metadata;

    RoboOSError(ErrorCode code, const std::string& message)
        : std::runtime_error(message), code(code) {}
    
    RoboOSError(ErrorCode code, const std::string& message, 
                const std::map<std::string, std::string>& metadata)
        : std::runtime_error(message), code(code), metadata(metadata) {}
};

class WalletError : public RoboOSError {
public:
    WalletError(ErrorCode code, const std::string& message)
        : RoboOSError(code, message) {}
    
    WalletError(ErrorCode code, const std::string& message,
                const std::map<std::string, std::string>& metadata)
        : RoboOSError(code, message, metadata) {}
};

class FPRError : public RoboOSError {
public:
    FPRError(ErrorCode code, const std::string& message)
        : RoboOSError(code, message) {}
    
    FPRError(ErrorCode code, const std::string& message,
             const std::map<std::string, std::string>& metadata)
        : RoboOSError(code, message, metadata) {}
};

class MarketplaceError : public RoboOSError {
public:
    MarketplaceError(ErrorCode code, const std::string& message)
        : RoboOSError(code, message) {}
    
    MarketplaceError(ErrorCode code, const std::string& message,
                     const std::map<std::string, std::string>& metadata)
        : RoboOSError(code, message, metadata) {}
};

class VerificationError : public RoboOSError {
public:
    VerificationError(ErrorCode code, const std::string& message)
        : RoboOSError(code, message) {}
    
    VerificationError(ErrorCode code, const std::string& message,
                      const std::map<std::string, std::string>& metadata)
        : RoboOSError(code, message, metadata) {}
};

class ReputationError : public RoboOSError {
public:
    ReputationError(ErrorCode code, const std::string& message)
        : RoboOSError(code, message) {}
    
    ReputationError(ErrorCode code, const std::string& message,
                    const std::map<std::string, std::string>& metadata)
        : RoboOSError(code, message, metadata) {}
};

} // namespace roboos

