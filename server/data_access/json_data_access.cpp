#include "../libs/json.hpp"
#include "json_data_access.hpp"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <chrono>

using json = nlohmann::json;

std::string JsonDataAccess::getFilePath(const std::string& user_key) const {
    // создаем data/users/user_key.json
    return data_dir + user_key + ".json";
}

std::vector<Record> JsonDataAccess::deleteExpired(std::vector<Record>& records) const {
    std::vector<Record> new_records;
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].expires_at > now) {
            new_records.push_back(records[i]);
        }
    }
    return new_records;
}

bool JsonDataAccess::save(const std::string& user_key, const Record& record) {
    std::vector<Record> all_records = onlyLoad(user_key);
    std::vector<Record> new_records = deleteExpired(all_records);

    new_records.push_back(record);
    return onlySave(user_key, new_records);
}

std::vector<Record> JsonDataAccess::load(const std::string& user_key) {
    std::vector<Record> all_records = onlyLoad(user_key);
    std::vector<Record> new_records = deleteExpired(all_records);

    onlySave(user_key, new_records);
    return new_records;
}

bool JsonDataAccess::onlySave(const std::string& user_key, std::vector<Record>& records) const {
    try {
        std::filesystem::create_directories(data_dir);
        
        json j_array = json::array();
        
        for (const auto& rec : records) {
            json record_json;
            record_json["data"] = rec.data;
            record_json["expires_at"] = rec.expires_at;
            j_array.push_back(record_json);
        }
        
        std::ofstream file(getFilePath(user_key));
        file << j_array.dump(4);
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка сохранения записи: " << e.what() << std::endl;
        return false;
    }
}

std::vector<Record> JsonDataAccess::onlyLoad(const std::string& user_key) const {
    std::vector<Record> records;
    std::string filepath = getFilePath(user_key);
    
    // файла нет - возвращаем пустой вектор
    if (!std::filesystem::exists(filepath)) {
        return records;
    }
    
    try {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл: " << filepath << std::endl;
            return records;
        }
        
        json j_array;
        file >> j_array;
        
        time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        for (const auto& item : j_array) {
            std::string data = item["data"];
            time_t expired_at = item["expires_at"];
            if (expired_at > now) {
                records.emplace_back(data, expired_at);
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки записей: " << e.what() << std::endl;
    }
    
    return records;
}