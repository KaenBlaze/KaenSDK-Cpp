#pragma once

#include <roboos/robots/BaseRobot.h>

namespace roboos {

struct DroneCapabilities {
    bool flight = false;
    bool surveillance = false;
    double maxAltitude = 0.0; // meters
    double maxSpeed = 0.0; // m/s
    double batteryCapacity = 0.0; // Wh
};

class DroneRobot : public BaseRobot {
private:
    DroneCapabilities droneCapabilities;

public:
    DroneRobot(const RobotOptions& options, const DroneCapabilities& capabilities);
};

} // namespace roboos

