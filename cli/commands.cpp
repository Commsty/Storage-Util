#include "commands.hpp"
#include <iostream>

int commandSave(const ParseResult& args, HttpClient& client) {
    try {
        const std::string response = client.postSave(args.text, args.ttlDays);
        std::cout << "Saved note (ttl " << args.ttlDays << "d). Server response:\n" << response << "\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Failed to save note: " << e.what() << "\n";
        return 1;
    }
}

int commandRead(const ParseResult&, HttpClient& client) {
    try {
        const std::string response = client.getAll();
        std::cout << "Notes:\n" << response << "\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Failed to read notes: " << e.what() << "\n";
        return 1;
    }
}
