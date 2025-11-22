#include <roboos/fpr/FPRClient.h>
#include <json/json.h>
#include <sstream>
#include <map>

namespace roboos {

FPRClient::FPRClient(const FPRConfig& config)
    : config(config), logger(getLogger()), httpClient(std::make_unique<CurlHttpClient>()) {
}

FPRClient::~FPRClient() {
    if (connected) {
        disconnect();
    }
}

void FPRClient::connect(const std::string& robotId, const std::string& robotPublicKey) {
    this->robotId = robotId;
    this->robotPublicKey = robotPublicKey;
    
    std::ostringstream url;
    url << config.endpoint << "/connect";
    
    Json::Value body;
    body["robotId"] = robotId;
    body["publicKey"] = robotPublicKey;
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->post(url.str(), bodyStr, headers, config.timeout);
    
    if (!response.isOk()) {
        throw FPRError(ErrorCode::FPR_CONNECTION_FAILED,
                      "FPR connection failed: " + std::to_string(response.statusCode));
    }
    
    connected = true;
    if (onConnected) {
        onConnected();
    }
    logger.info("Connected to FPR", {{"endpoint", config.endpoint}});
}

void FPRClient::disconnect() {
    if (!connected) {
        return;
    }
    
    std::ostringstream url;
    url << config.endpoint << "/disconnect";
    
    Json::Value body;
    body["robotId"] = robotId;
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    httpClient->post(url.str(), bodyStr, headers, config.timeout);
    
    connected = false;
    if (onDisconnected) {
        onDisconnected();
    }
    logger.info("Disconnected from FPR");
}

PaymentChannel FPRClient::openChannel(const OpenChannelRequest& request) {
    if (!connected) {
        throw FPRError(ErrorCode::FPR_CONNECTION_FAILED, "Not connected to FPR");
    }
    
    std::ostringstream url;
    url << config.endpoint << "/channels/open";
    
    Json::Value body;
    body["robotId"] = robotId;
    body["peerRobotId"] = request.peerRobotId;
    body["collateral"] = request.collateral;
    body["expirationTime"] = request.expirationTime;
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->post(url.str(), bodyStr, headers, config.timeout);
    
    if (!response.isOk()) {
        throw FPRError(ErrorCode::FPR_INSUFFICIENT_COLLATERAL,
                      "Failed to open channel: " + std::to_string(response.statusCode));
    }
    
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(response.body, root)) {
        throw FPRError(ErrorCode::FPR_CONNECTION_FAILED, "Failed to parse response");
    }
    
    PaymentChannel channel;
    channel.channelId = root["channelId"].asString();
    channel.peerRobotId = request.peerRobotId;
    channel.status = ChannelStatus::OPEN;
    channel.balance = 0.0;
    channel.collateral = request.collateral;
    channel.createdAt = root["createdAt"].asString();
    if (root.isMember("expiresAt")) {
        channel.expiresAt = root["expiresAt"].asString();
    }
    
    channels.push_back(channel);
    
    if (onChannelOpened) {
        onChannelOpened(channel);
    }
    
    logger.info("Payment channel opened", {{"channelId", channel.channelId}});
    
    return channel;
}

void FPRClient::sendPayment(const PaymentRequest& request) {
    if (!connected) {
        throw FPRError(ErrorCode::FPR_CONNECTION_FAILED, "Not connected to FPR");
    }
    
    std::ostringstream url;
    url << config.endpoint << "/channels/payment";
    
    Json::Value body;
    body["channelId"] = request.channelId;
    body["robotId"] = robotId;
    body["amount"] = request.amount;
    body["memo"] = request.memo;
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->post(url.str(), bodyStr, headers, config.timeout);
    
    if (!response.isOk()) {
        throw FPRError(ErrorCode::FPR_PAYMENT_FAILED,
                      "Payment failed: " + std::to_string(response.statusCode));
    }
    
    logger.debug("Payment sent", {{"channelId", request.channelId}, {"amount", std::to_string(request.amount)}});
}

void FPRClient::closeChannel(const std::string& channelId) {
    if (!connected) {
        throw FPRError(ErrorCode::FPR_CONNECTION_FAILED, "Not connected to FPR");
    }
    
    std::ostringstream url;
    url << config.endpoint << "/channels/close";
    
    Json::Value body;
    body["channelId"] = channelId;
    body["robotId"] = robotId;
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->post(url.str(), bodyStr, headers, config.timeout);
    
    if (!response.isOk()) {
        throw FPRError(ErrorCode::FPR_CONNECTION_FAILED,
                      "Failed to close channel: " + std::to_string(response.statusCode));
    }
    
    // Update channel status
    for (auto& channel : channels) {
        if (channel.channelId == channelId) {
            channel.status = ChannelStatus::CLOSED;
            break;
        }
    }
    
    if (onChannelClosed) {
        onChannelClosed(channelId);
    }
    
    logger.info("Payment channel closed", {{"channelId", channelId}});
}

ChannelStatus FPRClient::getChannelStatus(const std::string& channelId) {
    // Check local cache first
    for (const auto& channel : channels) {
        if (channel.channelId == channelId) {
            return channel.status;
        }
    }
    
    if (!connected) {
        throw FPRError(ErrorCode::FPR_CONNECTION_FAILED, "Not connected to FPR");
    }
    
    std::ostringstream url;
    url << config.endpoint << "/channels/" << channelId << "/status";
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->get(url.str(), headers, config.timeout);
    
    if (!response.isOk()) {
        throw FPRError(ErrorCode::FPR_CHANNEL_NOT_FOUND,
                      "Channel not found: " + channelId);
    }
    
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(response.body, root)) {
        throw FPRError(ErrorCode::FPR_CONNECTION_FAILED, "Failed to parse response");
    }
    
    return static_cast<ChannelStatus>(root["status"].asInt());
}

std::vector<PaymentChannel> FPRClient::getChannels() const {
    return channels;
}

} // namespace roboos

