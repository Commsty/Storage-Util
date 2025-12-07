#pragma once
#include <string>
#include <vector>
#include "../service/storage_service.hpp" // Для Record

class DataAccessLayer {
public:
    ~DataAccessLayer() = default;

    bool save(const std::string& key, const Record& data);

    std::vector<Record> load(const std::string& key);
};
