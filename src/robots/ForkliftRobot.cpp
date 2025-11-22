#include <roboos/robots/ForkliftRobot.h>
#include <thread>
#include <chrono>

namespace roboos {

ForkliftRobot::ForkliftRobot(const RobotOptions& options, const ForkliftCapabilities& capabilities)
    : BaseRobot(options), forkliftCapabilities(capabilities) {
}

bool ForkliftRobot::canHandleTask(const Task& task) const {
    if (task.type != TaskType::MATERIAL_HANDLING) {
        return false;
    }
    
    // Check weight requirements
    auto weightIt = task.metadata.find("maxWeight");
    if (weightIt != task.metadata.end()) {
        double requiredWeight = std::stod(weightIt->second);
        if (requiredWeight > forkliftCapabilities.maxWeight) {
            logger.debug("Task exceeds max weight");
            return false;
        }
    }
    
    // Check height requirements
    auto heightIt = task.metadata.find("maxHeight");
    if (heightIt != task.metadata.end()) {
        double requiredHeight = std::stod(heightIt->second);
        if (requiredHeight > forkliftCapabilities.maxHeight) {
            logger.debug("Task exceeds max height");
            return false;
        }
    }
    
    return true;
}

void ForkliftRobot::executeTask(const Task& task) {
    if (!canHandleTask(task)) {
        throw std::runtime_error("Cannot handle task: requirements not met");
    }
    
    logger.info("Executing forklift task", {{"taskId", task.taskId}});
    
    // In production, this would interface with the actual robot hardware
    // For now, simulate task execution
    std::this_thread::sleep_for(std::chrono::seconds(std::min(task.estimatedDuration, 5)));
    
    if (marketplace) {
        marketplace->updateTaskStatus(task.taskId, TaskStatus::COMPLETED);
    }
}

ForkliftCapabilities ForkliftRobot::getForkliftCapabilities() const {
    return forkliftCapabilities;
}

} // namespace roboos

