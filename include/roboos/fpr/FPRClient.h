#pragma once

#include <roboos/fpr/types.h>
#include <roboos/utils/errors.h>
#include <roboos/utils/logger.h>
#include <roboos/utils/http_client.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace roboos {

class FPRClient {
private:
    FPRConfig config;
    Logger& logger;
    std::unique_ptr<HttpClient> httpClient;
    std::string robotId;
    std::string robotPublicKey;
    bool connected = false;
    std::vector<PaymentChannel> channels;

public:
    FPRClient(const FPRConfig& config);
    ~FPRClient();
    
    void connect(const std::string& robotId, const std::string& robotPublicKey);
    void disconnect();
    
    PaymentChannel openChannel(const OpenChannelRequest& request);
    void sendPayment(const PaymentRequest& request);
    void closeChannel(const std::string& channelId);
    ChannelStatus getChannelStatus(const std::string& channelId);
    std::vector<PaymentChannel> getChannels() const;
    
    bool isConnected() const { return connected; }
    
    // Event callbacks
    std::function<void()> onConnected;
    std::function<void()> onDisconnected;
    std::function<void(const PaymentChannel&)> onChannelOpened;
    std::function<void(const std::string&)> onChannelClosed;
    std::function<void(const std::string&, double)> onPaymentReceived;
    std::function<void(const std::exception&)> onError;
};

} // namespace roboos

