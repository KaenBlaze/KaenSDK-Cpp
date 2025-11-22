#pragma once

#include <roboos/robots/BaseRobot.h>
#include <roboos/marketplace/types.h>
#include <string>

namespace roboos {

struct ForkliftCapabilities {
    bool lifting = false;
    bool transport = false;
    bool stacking = false;
    double maxWeight = 0.0; // kg
    double maxHeight = 0.0; // meters
    bool navigation = false;
};

class ForkliftRobot : public BaseRobot {
private:
    ForkliftCapabilities forkliftCapabilities;

public:
    ForkliftRobot(const RobotOptions& options, const ForkliftCapabilities& capabilities);
    
    bool canHandleTask(const Task& task) const;
    void executeTask(const Task& task);
    ForkliftCapabilities getForkliftCapabilities() const;
};

} // namespace roboos

