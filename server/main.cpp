#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

#include "libs/httplib.h"
#include "libs/json.hpp" 
#include "service/storage_service.hpp"
#include "data_access/json_data_access.hpp"

using json = nlohmann::json;

std::string format_notes(const std::vector<std::string>& notes) {
    if (notes.empty()) return "Notes list is empty.";
    std::ostringstream oss;
    for (const auto& n : notes) oss << "-- " << n << "\n";
    return oss.str();
}

int main() {
    JsonDataAccess dal;
    StorageService service(dal);
    httplib::Server svr;

    std::cout << "Server initialized." << std::endl;

    svr.Post("/save", [&](const httplib::Request& req, httplib::Response& res) {
        try {
            auto json_body = json::parse(req.body);
            
            std::string text;
            int ttl_days = 7;

            if (json_body.contains("data")) text = json_body["data"];
            if (json_body.contains("ttl")) ttl_days = json_body["ttl"];

            if (text.empty()) {
                res.status = 400;
                res.set_content("Error: Empty data", "text/plain");
                return;
            }

            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::hours(ttl_days * 24);
            time_t expires_at = std::chrono::system_clock::to_time_t(now + duration);

            std::cout << "[LOG] Saving: " << text << " (Expires: " << expires_at << ")" << std::endl;

            if (service.addRecord("default_user", text, expires_at)) {
                res.status = 200;
                res.set_content("Saved", "text/plain");
            } else {
                res.status = 500;
                res.set_content("Error", "text/plain");
            }

        } catch (...) {
            res.status = 400;
            res.set_content("JSON Error", "text/plain");
        }
    });

    svr.Get("/read", [&](const httplib::Request& req, httplib::Response& res) {
        auto notes = service.getAllRecords("default_user");
        res.set_content(format_notes(notes), "text/plain");
    });

    svr.listen("0.0.0.0", 8080);
    return 0;
}