#pragma once
#include <string>
#include <vector>
#include "service/storage_service.hpp" // Для Record

class DataAccessLayer {
public:
    ~DataAccessLayer() = default;

    // save сохраняет запись record для пользователя user_key
    bool save(const std::string& user_key, const Record& record);

    // load возвращает все записи пользователя user_key
    std::vector<Record> load(const std::string& user_key);
};
