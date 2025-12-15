#pragma once

#include <string>
#include <vector>
#include "data_access.hpp"

class JsonDataAccess : public DataAccessLayer {
private:
    std::string data_dir = "data/users/";
    
    std::string getFilePath(const std::string& user_key) const;
    
public:
    JsonDataAccess() = default;
    virtual ~JsonDataAccess() = default;
    
    bool save(const std::string& user_key, const Record& record) override;
    std::vector<Record> load(const std::string& user_key) override;
};