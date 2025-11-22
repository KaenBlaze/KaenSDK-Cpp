#pragma once

#include <string>
#include <vector>
#include <memory>

namespace roboos {

class WalletStorage {
public:
    virtual ~WalletStorage() = default;
    virtual bool exists() const = 0;
    virtual std::vector<uint8_t> load(const std::string& password = "") = 0;
    virtual void save(const std::vector<uint8_t>& keypair, bool encrypted, const std::string& password = "") = 0;
    virtual std::string backup() const = 0;
    virtual void restore(const std::string& backupData, const std::string& password = "") = 0;
};

class FileWalletStorage : public WalletStorage {
private:
    std::string path;
    std::vector<uint8_t> data;

public:
    FileWalletStorage(const std::string& path);
    bool exists() const override;
    std::vector<uint8_t> load(const std::string& password = "") override;
    void save(const std::vector<uint8_t>& keypair, bool encrypted, const std::string& password = "") override;
    std::string backup() const override;
    void restore(const std::string& backupData, const std::string& password = "") override;
};

class MemoryWalletStorage : public WalletStorage {
private:
    std::vector<uint8_t> data;
    bool exists_ = false;

public:
    bool exists() const override;
    std::vector<uint8_t> load(const std::string& password = "") override;
    void save(const std::vector<uint8_t>& keypair, bool encrypted, const std::string& password = "") override;
    std::string backup() const override;
    void restore(const std::string& backupData, const std::string& password = "") override;
};

std::unique_ptr<WalletStorage> createStorage(StorageType type, const std::string& path, bool encrypted, const std::string& password = "");

} // namespace roboos

