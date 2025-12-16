#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <chrono>

struct Record {
    std::string data; 
    time_t expires_at;

    Record (const std::string& d, time_t t) : data(d), expires_at(t) {}
};

class StorageService {
private:
    class DataAccessLayer& dal;

public:
    explicit StorageService(DataAccessLayer& dal_);

    bool addRecord(const std::string& user_key, const std::string& data, time_t expires_at);

    std::vector<std::string> getAllRecords(const std::string& user_key);
};