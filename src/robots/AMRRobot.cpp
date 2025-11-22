#include <roboos/robots/AMRRobot.h>
#include <thread>
#include <chrono>

namespace roboos {

AMRRobot::AMRRobot(const RobotOptions& options, const AMRCapabilities& capabilities)
    : BaseRobot(options), amrCapabilities(capabilities) {
}

bool AMRRobot::canHandleTask(const Task& task) const {
    return task.type == TaskType::TRANSPORT && amrCapabilities.navigation;
}

void AMRRobot::executeTask(const Task& task) {
    logger.info("Executing AMR task", {{"taskId", task.taskId}});
    std::this_thread::sleep_for(std::chrono::seconds(std::min(task.estimatedDuration, 5)));
    if (marketplace) {
        marketplace->updateTaskStatus(task.taskId, TaskStatus::COMPLETED);
    }
}

} // namespace roboos

