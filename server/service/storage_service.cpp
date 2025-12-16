#include "storage_service.hpp"
#include "../data_access/data_access.hpp"

StorageService::StorageService(DataAccessLayer& dal_) : dal(dal_) {}

bool StorageService::addRecord(const std::string& user_key, const std::string& data, time_t expires_at) {
    Record record(data, expires_at);
    return dal.save(user_key, record);
}

std::vector<std::string> StorageService::getAllRecords(const std::string& user_key) {
    auto records = dal.load(user_key);
    std::vector<std::string> result;
    
    auto now = std::chrono::system_clock::now();
    time_t current_time = std::chrono::system_clock::to_time_t(now);

    for (const auto& rec : records) {
        if (rec.expires_at > current_time) {
            result.push_back(rec.data);
        }
    }
    
    return result;
}