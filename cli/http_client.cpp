#include "http_client.hpp"
#include <curl/curl.h>
#include <stdexcept>
#include <sstream>
#include <iostream>

namespace {

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

std::string HttpClient::postSave(const std::string& text) {
    const std::string url = build_url("/save");
    return perform_request(url, &text);
}

std::string HttpClient::getAll() {
    const std::string url = build_url("/read");
    return perform_request(url, nullptr);
}

std::string HttpClient::build_url(const std::string& path) const {
    std::ostringstream os;
    os << "http://" << host_ << ":" << port_ << path;
    return os.str();
}

std::string HttpClient::perform_request(const std::string& url, const std::string* postData) {
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
        curl_global_cleanup();
        throw std::runtime_error("HTTP request failed: " + error);
    }

    long statusCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    if (statusCode >= 400) {
        std::ostringstream os;
        os << "Server returned HTTP status " << statusCode << " with body: " << responseData;
        throw std::runtime_error(os.str());
    }
    return responseData;
}