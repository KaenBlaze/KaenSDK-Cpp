#pragma once

#include <roboos/wallet/types.h>
#include <string>
#include <vector>

namespace roboos {

X402StealthAddress generateStealthAddress(const std::vector<uint8_t> &keypair);
bool verifyStealthAddress(const std::vector<uint8_t> &keypair,
                          const X402StealthAddress &address);

} // namespace roboos
