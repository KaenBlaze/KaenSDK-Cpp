#include <roboos/wallet/RobotWallet.h>
#include <roboos/wallet/WalletStorage.h>
#include <roboos/wallet/x402/x402.h>
#include <roboos/utils/logger.h>
#include <random>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace roboos {

RobotWallet::RobotWallet(std::unique_ptr<WalletStorage> storage, const WalletOptions& options)
    : storage(std::move(storage)), options(options), logger(getLogger()) {
}

RobotWallet::~RobotWallet() = default;

std::unique_ptr<RobotWallet> RobotWallet::create(const WalletOptions& options) {
    auto storage = createStorage(options.storage, options.path, options.encrypted, options.password);
    auto wallet = std::make_unique<RobotWallet>(std::move(storage), options);
    
    if (!wallet->storage->exists()) {
        // Generate new keypair
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        std::vector<uint8_t> keypair(64); // 32 bytes private + 32 bytes public
        for (auto& byte : keypair) {
            byte = dis(gen);
        }
        
        wallet->storage->save(keypair, options.encrypted, options.password);
        wallet->keypair = keypair;
        wallet->stealthAddress = generateStealthAddress(keypair);
        wallet->loaded = true;
        
        logger.info("New wallet created");
    } else {
        wallet->load();
    }
    
    return wallet;
}

void RobotWallet::load() {
    try {
        keypair = storage->load(options.password);
        stealthAddress = generateStealthAddress(keypair);
        loaded = true;
        logger.info("Wallet loaded");
    } catch (const WalletError& e) {
        throw;
    } catch (const std::exception& e) {
        throw WalletError(ErrorCode::WALLET_NOT_FOUND, 
                        "Failed to load wallet: " + std::string(e.what()));
    }
}

void RobotWallet::save() {
    if (keypair.empty()) {
        throw WalletError(ErrorCode::INVALID_KEYPAIR, "No keypair to save");
    }
    
    try {
        storage->save(keypair, options.encrypted, options.password);
        logger.debug("Wallet saved");
    } catch (const std::exception& e) {
        throw WalletError(ErrorCode::WALLET_ENCRYPTION_FAILED,
                         "Failed to save wallet: " + std::string(e.what()));
    }
}

std::string RobotWallet::getPublicKey() const {
    if (!loaded || keypair.empty()) {
        throw WalletError(ErrorCode::INVALID_KEYPAIR, "Wallet not loaded");
    }
    // Simplified - in production would extract public key from keypair
    return "pubkey_" + std::to_string(keypair.size());
}

X402StealthAddress RobotWallet::getStealthAddress() const {
    if (!loaded) {
        throw WalletError(ErrorCode::INVALID_KEYPAIR, "Wallet not loaded");
    }
    return stealthAddress;
}

double RobotWallet::getBalance(const std::string& rpcEndpoint) const {
    // Simplified - in production would query Solana RPC
    if (onBalanceChanged) {
        onBalanceChanged(0.0);
    }
    return 0.0;
}

WalletInfo RobotWallet::getInfo() const {
    if (!loaded) {
        throw WalletError(ErrorCode::INVALID_KEYPAIR, "Wallet not loaded");
    }
    
    WalletInfo info;
    info.publicKey = getPublicKey();
    info.address = stealthAddress.address;
    info.isEncrypted = options.encrypted;
    
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    info.createdAt = oss.str();
    
    return info;
}

std::string RobotWallet::backup() const {
    if (!loaded) {
        throw WalletError(ErrorCode::INVALID_KEYPAIR, "Wallet not loaded");
    }
    
    try {
        std::string backup = storage->backup();
        if (onBackupCreated) {
            onBackupCreated(backup);
        }
        logger.info("Wallet backup created");
        return backup;
    } catch (const std::exception& e) {
        throw WalletError(ErrorCode::WALLET_ENCRYPTION_FAILED,
                         "Failed to create backup: " + std::string(e.what()));
    }
}

void RobotWallet::restore(const std::string& backupData, const std::string& password) {
    try {
        storage->restore(backupData, password.empty() ? options.password : password);
        load();
        logger.info("Wallet restored from backup");
    } catch (const std::exception& e) {
        throw WalletError(ErrorCode::WALLET_DECRYPTION_FAILED,
                         "Failed to restore wallet: " + std::string(e.what()));
    }
}

bool RobotWallet::verifyStealthAddress(const X402StealthAddress& address) const {
    if (!loaded) {
        return false;
    }
    return roboos::verifyStealthAddress(keypair, address);
}

} // namespace roboos

