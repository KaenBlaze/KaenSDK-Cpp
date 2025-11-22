#include <roboos/wallet/WalletStorage.h>
#include <roboos/wallet/types.h>
#include <fstream>
#include <sstream>
#include <random>

namespace roboos {

FileWalletStorage::FileWalletStorage(const std::string& path) : path(path) {}

bool FileWalletStorage::exists() const {
    std::ifstream file(path);
    return file.good();
}

std::vector<uint8_t> FileWalletStorage::load(const std::string& password) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw WalletError(ErrorCode::WALLET_NOT_FOUND, "Wallet file not found: " + path);
    }
    
    std::vector<uint8_t> result((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
    // In production, decrypt here if password is provided
    return result;
}

void FileWalletStorage::save(const std::vector<uint8_t>& keypair, bool encrypted, const std::string& password) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw WalletError(ErrorCode::WALLET_ENCRYPTION_FAILED, "Failed to open wallet file for writing: " + path);
    }
    
    // In production, encrypt here if encrypted is true
    file.write(reinterpret_cast<const char*>(keypair.data()), keypair.size());
}

std::string FileWalletStorage::backup() const {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw WalletError(ErrorCode::WALLET_NOT_FOUND, "Wallet file not found for backup: " + path);
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

void FileWalletStorage::restore(const std::string& backupData, const std::string& password) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw WalletError(ErrorCode::WALLET_DECRYPTION_FAILED, "Failed to open wallet file for restore: " + path);
    }
    
    file.write(backupData.data(), backupData.size());
}

bool MemoryWalletStorage::exists() const {
    return exists_;
}

std::vector<uint8_t> MemoryWalletStorage::load(const std::string& password) {
    if (!exists_) {
        throw WalletError(ErrorCode::WALLET_NOT_FOUND, "Wallet not found in memory");
    }
    return data;
}

void MemoryWalletStorage::save(const std::vector<uint8_t>& keypair, bool encrypted, const std::string& password) {
    data = keypair;
    exists_ = true;
}

std::string MemoryWalletStorage::backup() const {
    if (!exists_) {
        throw WalletError(ErrorCode::WALLET_NOT_FOUND, "Wallet not found in memory for backup");
    }
    return std::string(reinterpret_cast<const char*>(data.data()), data.size());
}

void MemoryWalletStorage::restore(const std::string& backupData, const std::string& password) {
    data.assign(backupData.begin(), backupData.end());
    exists_ = true;
}

std::unique_ptr<WalletStorage> createStorage(StorageType type, const std::string& path, bool encrypted, const std::string& password) {
    if (type == StorageType::FILE) {
        return std::make_unique<FileWalletStorage>(path);
    } else {
        return std::make_unique<MemoryWalletStorage>();
    }
}

} // namespace roboos

