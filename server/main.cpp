#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "libs/httplib.h"
#include "service/storage_service.hpp"
#include "data_access/data_access.hpp"

std::string format_notes(const std::vector<std::string>& notes) {
    if (notes.empty()) return "Список заметок пуст.";
    std::ostringstream oss;
    for (const auto& n : notes) oss << "-- " << n << "\n";
    return oss.str();
}

int main() {
    DataAccessLayer dal;
    StorageService service(dal);

    httplib::Server svr;

    std::cout << "Server is starting..." << std::endl;

    svr.Post("/save", [&](const httplib::Request& req, httplib::Response& res) {
        std::string content = req.body;
        
        if (content.empty()) {
            res.status = 400;
            res.set_content("Empty note", "text/plain");
            return;
        }

        if (service.addRecord("default_user", content)) {
            std::cout << "[LOG] Saved note: " << content << std::endl;
            res.status = 200;
            res.set_content("Saved successfully", "text/plain");
        } else {
            res.status = 500;
            res.set_content("Server Error", "text/plain");
        }
    });

    svr.Get("/read", [&](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[LOG] Read request received" << std::endl;
        
        auto notes = service.getAllRecords("default_user");
        res.status = 200;
        res.set_content(format_notes(notes), "text/plain");
    });

    std::cout << "Server listening on port 8080..." << std::endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}