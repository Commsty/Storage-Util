#include "http_client.hpp"
#include <curl/curl.h>
#include <stdexcept>
#include <sstream>
#include <iostream>

namespace {

std::string escape_json(const std::string& value) {
    std::string escaped;
    escaped.reserve(value.size());
    for (char c : value) {
        switch (c) {
            case '\"':
                escaped += "\\\""; 
                break;
            case '\\': 
                escaped += "\\\\"; 
                break;
            case '\b': 
                escaped += "\\b"; 
                break;
            case '\f': 
                escaped += "\\f"; 
                break;
            case '\n': 
                escaped += "\\n"; 
                break;
            case '\r': 
                escaped += "\\r"; 
                break;
            case '\t': 
                escaped += "\\t"; 
                break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    // пропускаем другие символы чтобы не сломать json
                } else {
                    escaped += c;
                }
        }
    }
    return escaped;
}

size_t write_to_string(void* contents, size_t size, size_t nmemb, void* userp) {
    const size_t totalSize = size * nmemb;
    if (userp == nullptr || contents == nullptr) {
        return 0;
    }
    auto* buf = static_cast<std::string*>(userp);
    buf->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

}

HttpClient::HttpClient(std::string host, int port) : host_(std::move(host)), port_(port) {}

std::string HttpClient::postSave(const std::string& text, int ttlDays) {
    const std::string url = build_url("/save");
    std::ostringstream payload;
    payload << "{" << "\"data\":\"" << escape_json(text) << "\"," << "\"ttl\":" << ttlDays << "}";
    const std::string contentType = "Content-Type: application/json";
    const std::string payloadStr = payload.str();
    return perform_request(url, &payloadStr, &contentType);
}

std::string HttpClient::getAll() {
    const std::string url = build_url("/read");
    return perform_request(url, nullptr, nullptr);
}

std::string HttpClient::build_url(const std::string& path) const {
    std::ostringstream os;
    os << "http://" << host_ << ":" << port_ << path;
    return os.str();
}

std::string HttpClient::perform_request(const std::string& url, const std::string* postData, const std::string* contentType) {
    CURLcode global_init = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init != CURLE_OK) {
        throw std::runtime_error("Failed to initialize curl");
    }
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        throw std::runtime_error("Failed to create curl handle");
    }
    std::string responseData;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    struct curl_slist* headers = nullptr;
    if (contentType != nullptr) {
        headers = curl_slist_append(headers, contentType->c_str());
        if (headers != nullptr) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
    }
    if (postData != nullptr) {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData->c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(postData->size()));
    } else {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    }
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::string error = curl_easy_strerror(res);
        curl_easy_cleanup(curl);
        if (headers) {
            curl_slist_free_all(headers);
        }
        curl_global_cleanup();
        throw std::runtime_error("HTTP request failed: " + error);
    }

    long statusCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    curl_easy_cleanup(curl);
    if (headers) {
        curl_slist_free_all(headers);
    }
    curl_global_cleanup();

    if (statusCode >= 400) {
        std::ostringstream os;
        os << "Server returned HTTP status " << statusCode << " with body: " << responseData;
        throw std::runtime_error(os.str());
    }
    return responseData;
}