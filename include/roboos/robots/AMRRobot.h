#pragma once

#include <roboos/robots/BaseRobot.h>
#include <roboos/marketplace/types.h>

namespace roboos {

struct AMRCapabilities {
    bool navigation = false;
    bool obstacleAvoidance = false;
    double maxSpeed = 0.0; // m/s
    double maxPayload = 0.0; // kg
    double batteryCapacity = 0.0; // Wh
};

class AMRRobot : public BaseRobot {
private:
    AMRCapabilities amrCapabilities;

public:
    AMRRobot(const RobotOptions& options, const AMRCapabilities& capabilities);
    bool canHandleTask(const Task& task) const;
    void executeTask(const Task& task);
};

} // namespace roboos

