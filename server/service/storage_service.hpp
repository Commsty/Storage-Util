#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <chrono>

// Структура для хранения записи (пока без шифрования)
struct Record {
    std::string data;  // Данные как есть
    time_t created_at; // Время создания (?уникальный ID записи)

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

    // addRecord добавляет запись с текстом data пользователю user_key(username, userid, или что либо ещё)
    bool addRecord(const std::string& user_key, const std::string& data);

    // getAllRecords возвращает все записи пользователя user_key
    std::vector<std::string> getAllRecords(const std::string& user_key);
};