#include <roboos/robots/CleaningRobot.h>

namespace roboos {

CleaningRobot::CleaningRobot(const RobotOptions& options, const CleaningCapabilities& capabilities)
    : BaseRobot(options), cleaningCapabilities(capabilities) {
}

} // namespace roboos

