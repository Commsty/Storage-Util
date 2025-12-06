#pragma once
#include <string>

class HttpClient {
private:
    std::string build_url(const std::string& path) const;
    std::string perform_request(const std::string& url, const std::string* postData);
    std::string host_;
    int port_;

public:
    HttpClient(std::string host = "localhost", int port = 8080);
    std::string postSave(const std::string& text);
    std::string getAll();
};