#pragma once

#include <string>
#include <chrono>

namespace roboos {

enum class ChannelStatus {
    OPEN,
    ACTIVE,
    CLOSED,
    PENDING
};

struct PaymentChannel {
    std::string channelId;
    std::string peerRobotId;
    ChannelStatus status;
    double balance;
    double collateral;
    std::string createdAt;
    std::string expiresAt;
};

struct OpenChannelRequest {
    std::string peerRobotId;
    double collateral;
    std::string expirationTime;
};

struct PaymentRequest {
    std::string channelId;
    double amount;
    std::string memo;
};

struct FPRConfig {
    std::string endpoint;
    int timeout = 30000;
    int retryAttempts = 3;
    int retryDelay = 1000;
    int keepAliveInterval = 30000;
};

} // namespace roboos

