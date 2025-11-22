#include <roboos/marketplace/TaskMarketplace.h>
#include <roboos/utils/http_client.h>
#include <json/json.h>
#include <sstream>
#include <thread>
#include <chrono>
#include <map>

namespace roboos {

TaskMarketplace::TaskMarketplace(const MarketplaceConfig& config)
    : config(config), logger(getLogger()), httpClient(std::make_unique<CurlHttpClient>()) {
}

TaskMarketplace::~TaskMarketplace() {
    stopPolling();
}

void TaskMarketplace::setRobotId(const std::string& robotId) {
    this->robotId = robotId;
}

std::vector<Task> TaskMarketplace::queryTasks(const TaskQuery& query) {
    std::ostringstream url;
    url << config.endpoint << "/tasks?";
    
    if (query.type != TaskType::CUSTOM) {
        url << "type=" << static_cast<int>(query.type) << "&";
    }
    url << "status=" << static_cast<int>(query.status) << "&";
    url << "minReward=" << query.minReward << "&";
    url << "maxReward=" << query.maxReward << "&";
    url << "limit=" << query.limit << "&";
    url << "offset=" << query.offset;
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->get(url.str(), headers, config.timeout);
    
    if (!response.isOk()) {
        throw MarketplaceError(ErrorCode::NETWORK_ERROR,
                              "Failed to query tasks: " + std::to_string(response.statusCode));
    }
    
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(response.body, root)) {
        throw MarketplaceError(ErrorCode::NETWORK_ERROR, "Failed to parse response");
    }
    
    std::vector<Task> tasks;
    for (const auto& taskJson : root["tasks"]) {
        Task task;
        task.taskId = taskJson["taskId"].asString();
        task.type = static_cast<TaskType>(taskJson["type"].asInt());
        task.status = static_cast<TaskStatus>(taskJson["status"].asInt());
        task.description = taskJson["description"].asString();
        task.reward = taskJson["reward"].asDouble();
        task.estimatedDuration = taskJson["estimatedDuration"].asInt();
        task.createdAt = taskJson["createdAt"].asString();
        if (taskJson.isMember("deadline")) {
            task.deadline = taskJson["deadline"].asString();
        }
        tasks.push_back(task);
    }
    
    return tasks;
}

Task TaskMarketplace::getTask(const std::string& taskId) {
    std::ostringstream url;
    url << config.endpoint << "/tasks/" << taskId;
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->get(url.str(), headers, config.timeout);
    
    if (!response.isOk()) {
        if (response.statusCode == 404) {
            throw MarketplaceError(ErrorCode::TASK_NOT_FOUND, "Task not found: " + taskId);
        }
        throw MarketplaceError(ErrorCode::NETWORK_ERROR,
                              "Failed to get task: " + std::to_string(response.statusCode));
    }
    
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(response.body, root)) {
        throw MarketplaceError(ErrorCode::NETWORK_ERROR, "Failed to parse response");
    }
    
    Task task;
    task.taskId = root["taskId"].asString();
    task.type = static_cast<TaskType>(root["type"].asInt());
    task.status = static_cast<TaskStatus>(root["status"].asInt());
    task.description = root["description"].asString();
    task.reward = root["reward"].asDouble();
    task.estimatedDuration = root["estimatedDuration"].asInt();
    task.createdAt = root["createdAt"].asString();
    
    return task;
}

Bid TaskMarketplace::bid(const BidRequest& request) {
    if (robotId.empty()) {
        throw MarketplaceError(ErrorCode::INVALID_CONFIG, "Robot ID not set");
    }
    
    std::ostringstream url;
    url << config.endpoint << "/tasks/" << request.taskId << "/bids";
    
    Json::Value body;
    body["robotId"] = robotId;
    body["bidAmount"] = request.bidAmount;
    body["estimatedDuration"] = request.estimatedDuration;
    body["encrypted"] = request.encrypted;
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->post(url.str(), bodyStr, headers, config.timeout);
    
    if (!response.isOk()) {
        Json::Value errorRoot;
        Json::Reader reader;
        std::string errorMsg = "Bid rejected";
        if (reader.parse(response.body, errorRoot) && errorRoot.isMember("message")) {
            errorMsg = errorRoot["message"].asString();
        }
        
        if (response.statusCode == 400) {
            throw MarketplaceError(ErrorCode::INVALID_BID, errorMsg);
        }
        if (response.statusCode == 409) {
            throw MarketplaceError(ErrorCode::TASK_ALREADY_ASSIGNED, errorMsg);
        }
        throw MarketplaceError(ErrorCode::BID_REJECTED, errorMsg);
    }
    
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(response.body, root)) {
        throw MarketplaceError(ErrorCode::NETWORK_ERROR, "Failed to parse response");
    }
    
    Bid bid;
    bid.bidId = root["bidId"].asString();
    bid.taskId = request.taskId;
    bid.robotId = robotId;
    bid.bidAmount = request.bidAmount;
    bid.estimatedDuration = request.estimatedDuration;
    bid.submittedAt = root["submittedAt"].asString();
    bid.accepted = root.get("accepted", false).asBool();
    
    submittedBids.push_back(bid);
    if (onBidAccepted) {
        onBidAccepted(bid);
    }
    
    logger.info("Bid submitted", {{"taskId", request.taskId}, {"bidAmount", std::to_string(request.bidAmount)}});
    
    return bid;
}

Task TaskMarketplace::acceptTask(const std::string& taskId) {
    if (robotId.empty()) {
        throw MarketplaceError(ErrorCode::INVALID_CONFIG, "Robot ID not set");
    }
    
    std::ostringstream url;
    url << config.endpoint << "/tasks/" << taskId << "/accept";
    
    Json::Value body;
    body["robotId"] = robotId;
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->post(url.str(), bodyStr, headers, config.timeout);
    
    if (!response.isOk()) {
        throw MarketplaceError(ErrorCode::TASK_ALREADY_ASSIGNED,
                              "Failed to accept task: " + std::to_string(response.statusCode));
    }
    
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(response.body, root)) {
        throw MarketplaceError(ErrorCode::NETWORK_ERROR, "Failed to parse response");
    }
    
    Task task;
    task.taskId = root["taskId"].asString();
    task.type = static_cast<TaskType>(root["type"].asInt());
    task.status = static_cast<TaskStatus>(root["status"].asInt());
    
    if (onTaskAssigned) {
        onTaskAssigned(task);
    }
    
    logger.info("Task accepted", {{"taskId", taskId}});
    
    return task;
}

void TaskMarketplace::rejectTask(const std::string& taskId) {
    if (robotId.empty()) {
        throw MarketplaceError(ErrorCode::INVALID_CONFIG, "Robot ID not set");
    }
    
    std::ostringstream url;
    url << config.endpoint << "/tasks/" << taskId << "/reject";
    
    Json::Value body;
    body["robotId"] = robotId;
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->post(url.str(), bodyStr, headers, config.timeout);
    
    if (!response.isOk()) {
        throw MarketplaceError(ErrorCode::NETWORK_ERROR,
                              "Failed to reject task: " + std::to_string(response.statusCode));
    }
    
    logger.info("Task rejected", {{"taskId", taskId}});
}

void TaskMarketplace::updateTaskStatus(const std::string& taskId, TaskStatus status) {
    if (robotId.empty()) {
        throw MarketplaceError(ErrorCode::INVALID_CONFIG, "Robot ID not set");
    }
    
    std::ostringstream url;
    url << config.endpoint << "/tasks/" << taskId << "/status";
    
    Json::Value body;
    body["robotId"] = robotId;
    body["status"] = static_cast<int>(status);
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    auto response = httpClient->patch(url.str(), bodyStr, headers, config.timeout);
    
    if (!response.isOk()) {
        throw MarketplaceError(ErrorCode::NETWORK_ERROR,
                              "Failed to update task status: " + std::to_string(response.statusCode));
    }
    
    if (status == TaskStatus::COMPLETED && onTaskCompleted) {
        onTaskCompleted(taskId);
    }
    
    logger.debug("Task status updated", {{"taskId", taskId}, {"status", std::to_string(static_cast<int>(status))}});
}

void TaskMarketplace::startPolling(const TaskQuery& query) {
    if (polling) {
        return;
    }
    
    polling = true;
    // In production, this would run in a separate thread
    // For now, it's a placeholder
}

void TaskMarketplace::stopPolling() {
    polling = false;
}

std::vector<Bid> TaskMarketplace::getSubmittedBids() const {
    return submittedBids;
}

} // namespace roboos

