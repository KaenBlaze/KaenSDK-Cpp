# RoboOS Robot SDK - C++

C++ SDK for integrating robots into the **RoboOS ($ROS)** ecosystem. RoboOS is a Robotic Payment Operating System that enables robots to coordinate autonomously through economic incentives using blockchain Solana and the x402 stealth payment protocol.

## Features

- 🤖 **Robot Wallet SDK** - Solana wallet management with x402 stealth payment integration
- 💰 **Fleet Payment Router (FPR) Client** - Payment channels and micropayments
- 📋 **Task Marketplace API** - Task bidding, querying, and assignment
- 🔐 **Zero-Knowledge Verification** - Task completion proofs
- ⭐ **Reputation System** - Robot Reputation Ledger (RRL) integration
- 🏭 **Robot Type Helpers** - Pre-built helpers for Forklift, AMR, Cleaning, Hospital, Drone, and Robotic Arm

## Requirements

- C++17 or later
- CMake 3.20 or later
- libcurl (for HTTP requests)
- jsoncpp (for JSON parsing)
- Solana C++ bindings (optional, for direct blockchain interaction)

## Installation

### Dependencies

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libcurl4-openssl-dev libjsoncpp-dev
```

#### macOS
```bash
brew install cmake curl jsoncpp
```

### Building

```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

## Quick Start

```cpp
#include <roboos/RobotSDK.h>
#include <roboos/robots/ForkliftRobot.h>
#include <iostream>

int main() {
    // Initialize SDK
    roboos::RobotSDK::Config config;
    config.network = roboos::Network::TESTNET;
    config.fprEndpoint = "https://fpr-testnet.theroboos.com";
    config.marketplaceEndpoint = "https://marketplace-testnet.theroboos.com";
    
    roboos::RobotSDK sdk(config);
    
    // Create wallet
    roboos::WalletOptions walletOpts;
    walletOpts.storage = roboos::StorageType::FILE;
    walletOpts.path = "./robot-wallet.json";
    walletOpts.encrypted = true;
    
    auto wallet = roboos::RobotWallet::create(walletOpts);
    
    // Define forklift capabilities
    roboos::ForkliftCapabilities capabilities;
    capabilities.lifting = true;
    capabilities.transport = true;
    capabilities.stacking = true;
    capabilities.maxWeight = 2000; // kg
    capabilities.maxHeight = 5; // meters
    capabilities.navigation = true;
    
    // Create forklift robot
    roboos::RobotOptions robotOpts;
    robotOpts.robotId = "forklift-001";
    robotOpts.wallet = wallet;
    robotOpts.config = sdk.getConfig();
    
    roboos::ForkliftRobot forklift(robotOpts, capabilities);
    
    // Initialize and connect
    forklift.initialize();
    forklift.connect();
    
    std::cout << "Forklift robot connected" << std::endl;
    
    // Query tasks
    auto marketplace = forklift.getMarketplace();
    if (marketplace) {
        roboos::TaskQuery query;
        query.type = "material_handling";
        query.status = roboos::TaskStatus::PENDING;
        
        auto tasks = marketplace->queryTasks(query);
        std::cout << "Found " << tasks.size() << " tasks" << std::endl;
        
        for (const auto& task : tasks) {
            if (forklift.canHandleTask(task)) {
                marketplace->acceptTask(task.taskId);
                marketplace->updateTaskStatus(task.taskId, roboos::TaskStatus::IN_PROGRESS);
                
                forklift.executeTask(task);
                
                std::cout << "Task " << task.taskId << " completed" << std::endl;
            }
        }
    }
    
    forklift.disconnect();
    return 0;
}
```

## Compiling Your Program

```bash
g++ -std=c++17 your_program.cpp -lroboos-sdk -lcurl -ljsoncpp -o your_program
```

Or with CMake:

```cmake
find_package(roboos-sdk REQUIRED)
target_link_libraries(your_target roboos-sdk::roboos-sdk)
```

## Examples

See the [examples](./examples/) directory for complete examples:

- [Basic Robot](./examples/basic_robot.cpp) - Basic SDK usage
- [Forklift Example](./examples/forklift_example.cpp) - Forklift-specific capabilities
- [Bidding Example](./examples/bidding_example.cpp) - Custom bidding strategies
- [Payment Channel Example](./examples/payment_channel_example.cpp) - Payment channels and micropayments

## Robot Types

The SDK provides pre-built helpers for various robot types:

- **ForkliftRobot** - Material handling, warehouse operations
- **AMRRobot** - Navigation, delivery tasks
- **CleaningRobot** - Area coverage, route optimization
- **HospitalRobot** - Medical transport, sterilization
- **DroneRobot** - Flight routes, surveillance
- **RoboticArm** - Assembly, manipulation tasks

## Documentation

- [API Reference](./docs/API.md)
- [Getting Started Guide](./docs/guides/getting-started.md)
- [Wallet Setup](./docs/guides/wallet-setup.md)
- [Payment Channels](./docs/guides /payment-channels.md)
- [Task Bidding](./docs/guides/task-bidding.md)

## License

MIT

## Support

- Website: [theroboos.com](https://theroboos.com)
- GitHub: [github.com/roboos/robot-sdk-cpp](https://github.com/KaenBlaze/KaenSDK-Cpp)

For issues, questions, or contributions, please visit our [GitHub repository](https://github.com/KaenBlaze/KaenSDK-Cpp).

