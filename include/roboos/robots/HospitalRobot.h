#pragma once

#include <roboos/robots/BaseRobot.h>

namespace roboos {

struct HospitalCapabilities {
    bool medicalTransport = false;
    bool sterilization = false;
    bool temperatureControl = false;
};

class HospitalRobot : public BaseRobot {
private:
    HospitalCapabilities hospitalCapabilities;

public:
    HospitalRobot(const RobotOptions& options, const HospitalCapabilities& capabilities);
};

} // namespace roboos

