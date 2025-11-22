#include <roboos/robots/RoboticArm.h>

namespace roboos {

RoboticArm::RoboticArm(const RobotOptions& options, const RoboticArmCapabilities& capabilities)
    : BaseRobot(options), armCapabilities(capabilities) {
}

} // namespace roboos

