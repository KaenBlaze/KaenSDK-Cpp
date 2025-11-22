#pragma once

#include <roboos/fpr/types.h>
#include <vector>
#include <string>

namespace roboos {

class PaymentChannelManager {
private:
    std::vector<PaymentChannel> channels;

public:
    void addChannel(const PaymentChannel& channel);
    PaymentChannel* getChannel(const std::string& channelId);
    void updateBalance(const std::string& channelId, double newBalance);
    void updateStatus(const std::string& channelId, ChannelStatus status);
    ChannelStatus getChannelStatus(const std::string& channelId) const;
    std::vector<PaymentChannel> getAllChannels() const;
    void recordPaymentSent(const std::string& channelId, double amount);
};

} // namespace roboos

