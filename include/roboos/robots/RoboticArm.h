#pragma once

#include <roboos/robots/BaseRobot.h>

namespace roboos {

struct RoboticArmCapabilities {
    bool assembly = false;
    bool manipulation = false;
    double reach = 0.0; // meters
    double payload = 0.0; // kg
    int degreesOfFreedom = 0;
};

class RoboticArm : public BaseRobot {
private:
    RoboticArmCapabilities armCapabilities;

public:
    RoboticArm(const RobotOptions& options, const RoboticArmCapabilities& capabilities);
};

} // namespace roboos

