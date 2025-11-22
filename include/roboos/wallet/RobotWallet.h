#pragma once

#include <roboos/wallet/types.h>
#include <roboos/utils/errors.h>
#include <roboos/utils/logger.h>
#include <string>
#include <memory>
#include <functional>

namespace roboos {

class WalletStorage;

class RobotWallet {
private:
    std::unique_ptr<WalletStorage> storage;
    WalletOptions options;
    Logger& logger;
    std::vector<uint8_t> keypair; // Simplified - in real implementation would use proper keypair
    X402StealthAddress stealthAddress;
    bool loaded = false;

public:
    RobotWallet(std::unique_ptr<WalletStorage> storage, const WalletOptions& options);
    ~RobotWallet();
    
    static std::unique_ptr<RobotWallet> create(const WalletOptions& options);
    
    void load();
    void save();
    
    std::string getPublicKey() const;
    X402StealthAddress getStealthAddress() const;
    double getBalance(const std::string& rpcEndpoint = "") const;
    
    WalletInfo getInfo() const;
    std::string backup() const;
    void restore(const std::string& backupData, const std::string& password = "");
    bool verifyStealthAddress(const X402StealthAddress& address) const;
    
    // Event callbacks
    std::function<void(double)> onBalanceChanged;
    std::function<void(const std::string&)> onBackupCreated;
    std::function<void(const std::exception&)> onError;
};

} // namespace roboos

