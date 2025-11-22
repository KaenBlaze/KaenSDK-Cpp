#include <roboos/wallet/x402/x402.h>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace roboos {

X402StealthAddress generateStealthAddress(const std::vector<uint8_t>& keypair) {
    // Simplified implementation - in production would use proper x402 protocol
    X402StealthAddress address;
    
    // Generate address from keypair hash (simplified)
    std::ostringstream oss;
    oss << "x402_";
    for (size_t i = 0; i < std::min(keypair.size(), size_t(16)); ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(keypair[i]);
    }
    address.address = oss.str();
    
    if (keypair.size() >= 64) {
        address.publicKey = std::vector<uint8_t>(keypair.begin() + 32, keypair.end());
        address.privateKey = std::vector<uint8_t>(keypair.begin(), keypair.begin() + 32);
    } else {
        address.publicKey = keypair;
        address.privateKey = keypair;
    }
    
    return address;
}

bool verifyStealthAddress(const std::vector<uint8_t>& keypair, const X402StealthAddress& address) {
    auto generated = generateStealthAddress(keypair);
    return generated.address == address.address;
}

} // namespace roboos

