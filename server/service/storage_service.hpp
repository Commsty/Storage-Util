#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <chrono>

struct Record {
    std::string data; 
    time_t created_at;

    Record (const std::string& d, time_t t = 0) : data(d), created_at(t) {
        if (created_at == 0) {
            auto now = std::chrono::system_clock::now();
            created_at = std::chrono::system_clock::to_time_t(now);
        }
    }
};

class StorageService {
private:
    class DataAccessLayer& dal;

public:
    explicit StorageService(DataAccessLayer& dal_);

    bool addRecord(const std::string& user_key, const std::string& data, time_t expires_at);

    std::vector<std::string> getAllRecords(const std::string& user_key);
};