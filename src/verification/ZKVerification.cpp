#include <roboos/verification/ZKVerification.h>
#include <roboos/utils/http_client.h>
#include <json/json.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <map>

namespace roboos {

ZKVerification::ZKVerification(const VerificationConfig& config)
    : config(config), logger(getLogger()) {
}

TaskProof ZKVerification::generateProof(const ProofGenerationOptions& options) {
    TaskProof proof;
    proof.taskId = options.taskId;
    proof.robotId = options.robotId;
    
    // Generate proof ID
    std::ostringstream proofId;
    proofId << "proof_" << options.taskId << "_" << options.robotId;
    proof.proofId = proofId.str();
    
    // In production, this would generate actual ZK proof
    proof.proofData = "zk_proof_data_placeholder";
    
    if (options.includeTimestamp) {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        proof.timestamp = oss.str();
    }
    
    return proof;
}

void ZKVerification::submitProof(const TaskProof& proof) {
    std::ostringstream url;
    url << config.endpoint << "/proofs";
    
    Json::Value body;
    body["proofId"] = proof.proofId;
    body["taskId"] = proof.taskId;
    body["robotId"] = proof.robotId;
    body["proofData"] = proof.proofData;
    body["timestamp"] = proof.timestamp;
    
    Json::StreamWriterBuilder builder;
    std::string bodyStr = Json::writeString(builder, body);
    
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    
    CurlHttpClient httpClient;
    auto response = httpClient.post(url.str(), bodyStr, headers, config.timeout);
    
    if (!response.isOk()) {
        throw VerificationError(ErrorCode::VERIFICATION_FAILED,
                                "Failed to submit proof: " + std::to_string(response.statusCode));
    }
    
    logger.info("Proof submitted", {{"proofId", proof.proofId}});
}

} // namespace roboos

