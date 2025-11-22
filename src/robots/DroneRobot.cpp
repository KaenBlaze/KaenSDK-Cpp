#include <roboos/robots/DroneRobot.h>

namespace roboos {

DroneRobot::DroneRobot(const RobotOptions& options, const DroneCapabilities& capabilities)
    : BaseRobot(options), droneCapabilities(capabilities) {
}

} // namespace roboos

