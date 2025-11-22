#include <roboos/utils/http_client.h>
#include <curl/curl.h>
#include <sstream>
#include <mutex>

namespace roboos {

class CurlHttpClient::Impl {
public:
    Impl() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }
    
    ~Impl() {
        curl_global_cleanup();
    }
    
    CURL* createCurlHandle() {
        return curl_easy_init();
    }
    
    void cleanupCurlHandle(CURL* handle) {
        if (handle) {
            curl_easy_cleanup(handle);
        }
    }
};

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    size_t totalSize = size * nmemb;
    data->append((char*)contents, totalSize);
    return totalSize;
}

static size_t HeaderCallback(char* buffer, size_t size, size_t nitems, std::map<std::string, std::string>* headers) {
    size_t totalSize = size * nitems;
    std::string header(buffer, totalSize);
    
    size_t colonPos = header.find(':');
    if (colonPos != std::string::npos) {
        std::string key = header.substr(0, colonPos);
        std::string value = header.substr(colonPos + 1);
        
        // Trim whitespace
        while (!value.empty() && (value[0] == ' ' || value[0] == '\t')) {
            value.erase(0, 1);
        }
        while (!value.empty() && (value.back() == '\r' || value.back() == '\n')) {
            value.pop_back();
        }
        
        (*headers)[key] = value;
    }
    
    return totalSize;
}

CurlHttpClient::CurlHttpClient() : pImpl(std::make_unique<Impl>()) {}

CurlHttpClient::~CurlHttpClient() = default;

HttpResponse CurlHttpClient::get(const std::string& url,
                                 const std::map<std::string, std::string>& headers,
                                 int timeoutMs) {
    CURL* curl = pImpl->createCurlHandle();
    if (!curl) {
        return HttpResponse{500, "", {}};
    }
    
    std::string responseBody;
    std::map<std::string, std::string> responseHeaders;
    long statusCode = 0;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeaders);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    // Add custom headers
    struct curl_slist* headerList = nullptr;
    for (const auto& [key, value] : headers) {
        std::string header = key + ": " + value;
        headerList = curl_slist_append(headerList, header.c_str());
    }
    if (headerList) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    }
    
    if (headerList) {
        curl_slist_free_all(headerList);
    }
    pImpl->cleanupCurlHandle(curl);
    
    return HttpResponse{static_cast<int>(statusCode), responseBody, responseHeaders};
}

HttpResponse CurlHttpClient::post(const std::string& url,
                                 const std::string& body,
                                 const std::map<std::string, std::string>& headers,
                                 int timeoutMs) {
    CURL* curl = pImpl->createCurlHandle();
    if (!curl) {
        return HttpResponse{500, "", {}};
    }
    
    std::string responseBody;
    std::map<std::string, std::string> responseHeaders;
    long statusCode = 0;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeaders);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs);
    
    struct curl_slist* headerList = nullptr;
    for (const auto& [key, value] : headers) {
        std::string header = key + ": " + value;
        headerList = curl_slist_append(headerList, header.c_str());
    }
    if (headerList) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    }
    
    if (headerList) {
        curl_slist_free_all(headerList);
    }
    pImpl->cleanupCurlHandle(curl);
    
    return HttpResponse{static_cast<int>(statusCode), responseBody, responseHeaders};
}

HttpResponse CurlHttpClient::patch(const std::string& url,
                                   const std::string& body,
                                   const std::map<std::string, std::string>& headers,
                                   int timeoutMs) {
    CURL* curl = pImpl->createCurlHandle();
    if (!curl) {
        return HttpResponse{500, "", {}};
    }
    
    std::string responseBody;
    std::map<std::string, std::string> responseHeaders;
    long statusCode = 0;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeaders);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs);
    
    struct curl_slist* headerList = nullptr;
    for (const auto& [key, value] : headers) {
        std::string header = key + ": " + value;
        headerList = curl_slist_append(headerList, header.c_str());
    }
    if (headerList) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    }
    
    if (headerList) {
        curl_slist_free_all(headerList);
    }
    pImpl->cleanupCurlHandle(curl);
    
    return HttpResponse{static_cast<int>(statusCode), responseBody, responseHeaders};
}

HttpResponse CurlHttpClient::del(const std::string& url,
                                 const std::map<std::string, std::string>& headers,
                                 int timeoutMs) {
    CURL* curl = pImpl->createCurlHandle();
    if (!curl) {
        return HttpResponse{500, "", {}};
    }
    
    std::string responseBody;
    std::map<std::string, std::string> responseHeaders;
    long statusCode = 0;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeaders);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs);
    
    struct curl_slist* headerList = nullptr;
    for (const auto& [key, value] : headers) {
        std::string header = key + ": " + value;
        headerList = curl_slist_append(headerList, header.c_str());
    }
    if (headerList) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    }
    
    if (headerList) {
        curl_slist_free_all(headerList);
    }
    pImpl->cleanupCurlHandle(curl);
    
    return HttpResponse{static_cast<int>(statusCode), responseBody, responseHeaders};
}

} // namespace roboos

