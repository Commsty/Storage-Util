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

std::vector<Record> JsonDataAccess::onlyLoad(const std::string& user_key) const {
    
}

bool JsonDataAccess::save(const std::string& user_key, const Record& record) {
    try {
        std::filesystem::create_directories(data_dir);
        
        std::vector<Record> all_records = load(user_key);
        
        all_records.push_back(record);
        
        json j_array = json::array();
        
        for (const auto& rec : all_records) {
            json record_json;
            record_json["data"] = rec.data;
            record_json["expired_at"] = rec.expired_at;  
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

std::vector<Record> JsonDataAccess::load(const std::string& user_key) {
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
            time_t expired_at = item["expired_at"];
            if (expired_at > now) {
                records.emplace_back(data, expired_at);
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки записей: " << e.what() << std::endl;
    }
    
    return records;
}