#include <roboos/robots/HospitalRobot.h>

namespace roboos {

HospitalRobot::HospitalRobot(const RobotOptions& options, const HospitalCapabilities& capabilities)
    : BaseRobot(options), hospitalCapabilities(capabilities) {
}

} // namespace roboos

