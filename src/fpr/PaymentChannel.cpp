#include <roboos/fpr/PaymentChannel.h>

namespace roboos {

void PaymentChannelManager::addChannel(const PaymentChannel& channel) {
    channels.push_back(channel);
}

PaymentChannel* PaymentChannelManager::getChannel(const std::string& channelId) {
    for (auto& channel : channels) {
        if (channel.channelId == channelId) {
            return &channel;
        }
    }
    return nullptr;
}

void PaymentChannelManager::updateBalance(const std::string& channelId, double newBalance) {
    for (auto& channel : channels) {
        if (channel.channelId == channelId) {
            channel.balance = newBalance;
            break;
        }
    }
}

void PaymentChannelManager::updateStatus(const std::string& channelId, ChannelStatus status) {
    for (auto& channel : channels) {
        if (channel.channelId == channelId) {
            channel.status = status;
            break;
        }
    }
}

ChannelStatus PaymentChannelManager::getChannelStatus(const std::string& channelId) const {
    for (const auto& channel : channels) {
        if (channel.channelId == channelId) {
            return channel.status;
        }
    }
    return ChannelStatus::CLOSED;
}

std::vector<PaymentChannel> PaymentChannelManager::getAllChannels() const {
    return channels;
}

void PaymentChannelManager::recordPaymentSent(const std::string& channelId, double amount) {
    // Record payment for accounting purposes
    // In production, this would update internal accounting
}

} // namespace roboos

