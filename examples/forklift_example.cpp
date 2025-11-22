#include <roboos/RobotSDK.h>
#include <roboos/robots/ForkliftRobot.h>
#include <roboos/wallet/RobotWallet.h>
#include <iostream>

int main() {
    try {
        // Initialize SDK
        roboos::SDKOptions sdkOptions;
        sdkOptions.network = roboos::Network::TESTNET;
        sdkOptions.fprEndpoint = "https://fpr-testnet.theroboos.com";
        sdkOptions.marketplaceEndpoint = "https://marketplace-testnet.theroboos.com";
        
        roboos::RobotSDK sdk(sdkOptions);
        
        // Create wallet
        roboos::WalletOptions walletOpts;
        walletOpts.storage = roboos::StorageType::FILE;
        walletOpts.path = "./forklift-wallet.json";
        walletOpts.encrypted = true;
        
        auto wallet = roboos::RobotWallet::create(walletOpts);
        
        // Define forklift capabilities
        roboos::ForkliftCapabilities capabilities;
        capabilities.lifting = true;
        capabilities.transport = true;
        capabilities.stacking = true;
        capabilities.maxWeight = 2000.0; // kg
        capabilities.maxHeight = 5.0; // meters
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
            query.type = roboos::TaskType::MATERIAL_HANDLING;
            query.status = roboos::TaskStatus::PENDING;
            
            auto tasks = marketplace->queryTasks(query);
            std::cout << "Found " << tasks.size() << " material handling tasks" << std::endl;
            
            for (const auto& task : tasks) {
                if (forklift.canHandleTask(task)) {
                    std::cout << "Forklift can handle task: " << task.taskId << std::endl;
                    
                    marketplace->acceptTask(task.taskId);
                    marketplace->updateTaskStatus(task.taskId, roboos::TaskStatus::IN_PROGRESS);
                    
                    forklift.executeTask(task);
                    
                    std::cout << "Task " << task.taskId << " completed" << std::endl;
                } else {
                    std::cout << "Forklift cannot handle task: " << task.taskId << std::endl;
                }
            }
        }
        
        forklift.disconnect();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

