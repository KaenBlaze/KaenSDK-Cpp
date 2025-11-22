#pragma once

#include <roboos/marketplace/types.h>
#include <roboos/utils/errors.h>
#include <roboos/utils/logger.h>
#include <roboos/utils/http_client.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace roboos {

struct MarketplaceConfig {
    std::string endpoint;
    int timeout = 30000;
    int retryAttempts = 3;
    int retryDelay = 1000;
    int pollingInterval = 60000;
};

class TaskMarketplace {
private:
    MarketplaceConfig config;
    Logger& logger;
    std::unique_ptr<HttpClient> httpClient;
    std::string robotId;
    std::vector<Bid> submittedBids;
    bool polling = false;

public:
    TaskMarketplace(const MarketplaceConfig& config);
    ~TaskMarketplace();
    
    void setRobotId(const std::string& robotId);
    
    std::vector<Task> queryTasks(const TaskQuery& query = TaskQuery());
    Task getTask(const std::string& taskId);
    Bid bid(const BidRequest& request);
    Task acceptTask(const std::string& taskId);
    void rejectTask(const std::string& taskId);
    void updateTaskStatus(const std::string& taskId, TaskStatus status);
    
    void startPolling(const TaskQuery& query = TaskQuery());
    void stopPolling();
    std::vector<Bid> getSubmittedBids() const;
    
    // Event callbacks
    std::function<void(const Task&)> onTaskAvailable;
    std::function<void(const Task&)> onTaskAssigned;
    std::function<void(const Bid&)> onBidAccepted;
    std::function<void(const Bid&)> onBidRejected;
    std::function<void(const std::string&)> onTaskCompleted;
    std::function<void(const std::exception&)> onError;
};

} // namespace roboos

