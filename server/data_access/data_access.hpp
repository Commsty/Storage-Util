#pragma once
#include <string>
#include <optional>
#include "../service/storage_service.hpp" // Для StoredData

class DataAccessLayer {
public:
    virtual ~DataAccessLayer() = default;

    virtual bool save(const std::string& key, const StoredData& data) = 0;

    virtual std::optional<StoredData> load(const std::string& key) = 0;
};
