
#include "data_access.hpp"

//заглушка

bool DataAccessLayer::save(const std::string& user_key, const Record& record) {
    return false;
}

std::vector<Record> DataAccessLayer::load(const std::string& user_key) {
    return std::vector<Record>();
}
