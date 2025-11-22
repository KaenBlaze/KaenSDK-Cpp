#include <roboos/reputation/ReputationClient.h>
#include <roboos/utils/http_client.h>
#include <json/json.h>
#include <map>

namespace roboos {

ReputationClient::ReputationClient(const ReputationConfig& config)
    : config(config), logger(getLogger()) {
}

ReputationScore ReputationClient::getScore(const std::string& robotId) {
    std::ostringstream url;
    url << config.endpoint << "/reputation/" << robotId;
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    CurlHttpClient httpClient;
    auto response = httpClient.get(url.str(), headers, config.timeout);
    
    if (!response.isOk()) {
        throw ReputationError(ErrorCode::REPUTATION_QUERY_FAILED,
                             "Failed to get reputation: " + std::to_string(response.statusCode));
    }
    
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(response.body, root)) {
        throw ReputationError(ErrorCode::REPUTATION_QUERY_FAILED, "Failed to parse response");
    }
    
    ReputationScore score;
    score.robotId = robotId;
    score.score = root["score"].asDouble();
    score.completedTasks = root["completedTasks"].asInt();
    score.totalTasks = root["totalTasks"].asInt();
    score.lastUpdated = root["lastUpdated"].asString();
    
    return score;
}

void ReputationClient::submitUpdate(const ReputationUpdate& update) {
    std::ostringstream url;
    url << config.endpoint << "/reputation/update";
    
    Json::Value body;
    body["robotId"] = update.robotId;
    body["taskId"] = update.taskId;
    body["success"] = update.success;
    body["timestamp"] = update.timestamp;
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    CurlHttpClient httpClient;
    auto response = httpClient.post(url.str(), bodyStr, headers, config.timeout);
    
    if (!response.isOk()) {
        throw ReputationError(ErrorCode::REPUTATION_UPDATE_FAILED,
                             "Failed to submit update: " + std::to_string(response.statusCode));
    }
    
    logger.info("Reputation update submitted", {{"robotId", update.robotId}});
}

void ReputationClient::destroy() {
    // Cleanup if needed
}

} // namespace roboos

