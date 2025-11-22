#pragma once

#include <string>
#include <map>
#include <vector>
#include <chrono>

namespace roboos {

enum class TaskStatus {
    PENDING,
    ASSIGNED,
    IN_PROGRESS,
    COMPLETED,
    FAILED,
    CANCELLED
};

enum class TaskType {
    MATERIAL_HANDLING,
    TRANSPORT,
    CLEANING,
    SURVEILLANCE,
    ASSEMBLY,
    CUSTOM
};

struct Task {
    std::string taskId;
    TaskType type;
    TaskStatus status;
    std::string description;
    double reward;
    int estimatedDuration; // seconds
    std::map<std::string, std::string> metadata;
    std::vector<std::string> requirements;
    std::string location;
    std::string createdAt;
    std::string deadline;
    std::string completedAt;
    std::string assignedRobotId;
};

struct Bid {
    std::string bidId;
    std::string taskId;
    std::string robotId;
    double bidAmount;
    int estimatedDuration;
    std::string submittedAt;
    bool accepted = false;
};

struct BidRequest {
    std::string taskId;
    double bidAmount;
    int estimatedDuration;
    bool encrypted = false;
};

struct TaskQuery {
    TaskType type;
    TaskStatus status;
    double minReward = 0.0;
    double maxReward = 0.0;
    int limit = 100;
    int offset = 0;
    std::string location;
};

} // namespace roboos

