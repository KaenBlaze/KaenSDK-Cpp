#pragma once

#include <string>
#include <memory>
#include <vector>

namespace roboos {

enum class StorageType {
    FILE,
    MEMORY
};

struct X402StealthAddress {
    std::string address;
    std::vector<uint8_t> publicKey;
    std::vector<uint8_t> privateKey;
};

struct WalletInfo {
    std::string publicKey;
    std::string address;
    bool isEncrypted;
    std::string createdAt;
};

struct WalletOptions {
    StorageType storage = StorageType::FILE;
    std::string path;
    bool encrypted = false;
    std::string password;
    bool autoBackup = false;
    int backupInterval = 3600000; // 1 hour in ms
};

} // namespace roboos

