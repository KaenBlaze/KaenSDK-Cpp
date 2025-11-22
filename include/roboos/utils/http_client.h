#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

namespace roboos {

struct HttpResponse {
    int statusCode;
    std::string body;
    std::map<std::string, std::string> headers;
    
    bool isOk() const { return statusCode >= 200 && statusCode < 300; }
};

class HttpClient {
public:
    virtual ~HttpClient() = default;
    
    virtual HttpResponse get(const std::string& url,
                            const std::map<std::string, std::string>& headers = {},
                            int timeoutMs = 30000) = 0;
    
    virtual HttpResponse post(const std::string& url,
                             const std::string& body,
                             const std::map<std::string, std::string>& headers = {},
                             int timeoutMs = 30000) = 0;
    
    virtual HttpResponse patch(const std::string& url,
                               const std::string& body,
                               const std::map<std::string, std::string>& headers = {},
                               int timeoutMs = 30000) = 0;
    
    virtual HttpResponse del(const std::string& url,
                            const std::map<std::string, std::string>& headers = {},
                            int timeoutMs = 30000) = 0;
};

class CurlHttpClient : public HttpClient {
public:
    CurlHttpClient();
    ~CurlHttpClient();
    
    HttpResponse get(const std::string& url,
                    const std::map<std::string, std::string>& headers = {},
                    int timeoutMs = 30000) override;
    
    HttpResponse post(const std::string& url,
                     const std::string& body,
                     const std::map<std::string, std::string>& headers = {},
                     int timeoutMs = 30000) override;
    
    HttpResponse patch(const std::string& url,
                      const std::string& body,
                      const std::map<std::string, std::string>& headers = {},
                      int timeoutMs = 30000) override;
    
    HttpResponse del(const std::string& url,
                    const std::map<std::string, std::string>& headers = {},
                    int timeoutMs = 30000) override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace roboos

