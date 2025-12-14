#include <iostream>
#include "data_access/json_data_access.hpp"

int main() {
    std::cout << "Тест финальный\n";
    std::cout << "================================\n";
    
    JsonDataAccess dal;
    
    Record test_record("Привет от Евы! Это тестовая запись.");
    std::cout << "1)Сохраняем запись...\n";
    
    if (dal.save("eva_final", test_record)) {
        std::cout << "сохранился\n";
    } else {
        std::cout << "не сохранился из-за ошибки\n";
        return 1;
    }
    
    std::cout << "\n2) Загружаем записи...\n";
    auto records = dal.load("eva_final");
    
    if (records.empty()) {
        std::cout << "какая-то ошибка загрузки((((\n";
        return 1;
    }
    
    std::cout << "загружено записей:" << records.size() << "\n";
    
    std::cout << "\n3) Проверяем содержимое...\n";
    for (size_t i = 0; i < records.size(); i++) {
        std::cout << "   Запись #" << (i+1) << ": \"" 
                  << records[i].data << "\"\n";
        std::cout << "   Время создания: " 
                  << records[i].created_at << "\n";
    }
    

    std::cout << "\n4) Файл должен быть создан:\n";
    std::cout << "   data/users/eva_final.json\n\n";
    

    std::cout << "5) Содержимое файла:\n";
    system("cat data/users/eva_final.json 2>/dev/null || echo 'файл не найден'");
    
    std::cout << "\nВСЁ РАБОТАЕТ!!!!!!!!!!!!!!\n";
    return 0;
}
