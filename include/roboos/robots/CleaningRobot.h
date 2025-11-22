#pragma once

#include <roboos/robots/BaseRobot.h>

namespace roboos {

struct CleaningCapabilities {
    bool vacuum = false;
    bool mopping = false;
    double coverageArea = 0.0; // m²
    double batteryCapacity = 0.0; // Wh
};

class CleaningRobot : public BaseRobot {
private:
    CleaningCapabilities cleaningCapabilities;

public:
    CleaningRobot(const RobotOptions& options, const CleaningCapabilities& capabilities);
};

} // namespace roboos

