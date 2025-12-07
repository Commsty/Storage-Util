#include "storage_service.hpp"
#include "../data_access/data_access.hpp"

StorageService::StorageService(DataAccessLayer& dal_) : dal(dal_) {}

bool StorageService::addRecord(const std::string& user_key, const std::string& data) {
    Record record(data);
    return dal.save(user_key, record);
}

std::vector<std::string> StorageService::getAllRecords(const std::string& user_key) {
    auto records = dal.load(user_key);

    std::vector<std::string> result(records.size());
    for (size_t i = 0; i < records.size(); ++i) {
        result[i] = records[i].data;
    }
    
    return result;
}