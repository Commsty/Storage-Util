#include "args_parser.hpp"
#include <iostream>
#include <vector>

namespace {

std::vector<std::string> to_args(int argc, char* argv[]) {
    std::vector<std::string> args;
    const size_t argsCount = (argc > 1) ? static_cast<size_t>(argc - 1) : 0;
    args.reserve(argsCount);
    for (int i = 1; i < argc; ++i) {
        if (argv[i] != nullptr) {
            args.emplace_back(argv[i]);
        }
    }
    return args;
}

bool parse_ttl_flag(const std::string& arg, int& ttlDays, std::string& error) {
    const std::string prefix = "--ttl=";
    if (arg.rfind(prefix, 0) != 0) {
        return false;
    }
    const std::string value = arg.substr(prefix.size());
    if (value.size() < 2 || value.back() != 'd') {
        error = "Invalid ttl format. Use --ttl=<days>d";
        return true;
    }
    const std::string number_part = value.substr(0, value.size() - 1);
    if (number_part.empty()) {
        error = "Invalid ttl format. Use --ttl=<days>d";
        return true;
    }
    try {
        int days = std::stoi(number_part);
        if (days <= 0) {
            error = "Ttl must be a positive number of days";
            return true;
        }
        ttlDays = days;
    } catch (const std::exception&) {
        error = "Invalid ttl format. Use --ttl=<days>d";
    }
    return true;
}

}

ParseResult ArgsParser::parse(int argc, char* argv[]) const {
    ParseResult result;
    const auto args = to_args(argc, argv);
    if (args.empty()) {
        result.command = Help;
        return result;
    }
    const std::string& command = args[0];
    if (command == "--help") {
        result.command = Help;
        return result;
    }
    if (command == "--save") {
        result.command = Save;
        result.ttlDays = 7;
        for (size_t i = 1; i < args.size(); ++i) {
            const std::string& param = args[i];
            if (param.rfind("--ttl=", 0) == 0) {
                std::string ttlError;
                parse_ttl_flag(param, result.ttlDays, ttlError);
                if (!ttlError.empty()) {
                    result.errorMessage = ttlError;
                    return result;
                }
            } else if (result.text.empty()) {
                result.text = param;
            } else {
                result.errorMessage = "Too many arguments for --save";
                return result;
            }
        }
        if (result.text.empty()) {
            result.errorMessage = "Missing text for --save";
        }
        return result;
    }
    if (command == "--read") {
        if (args.size() > 1) {
            result.errorMessage = "Unexpected arguments for --read";
        }
        result.command = Read;
        return result;
    }
    result.errorMessage = "Unknown command: " + command;
    return result;
}

void ArgsParser::printHelp(const std::string& programName) {
    std::cout << "Usage:\n"
              << "  " << programName << " [command] [options]\n\n"
              << "Commands:\n"
              << "  --save (--ttl=<days>d) \"<text>\"   Store a text record on the server\n"
              << "  --read            Show all stored records\n"
              << "  --help            Display help information\n\n"
              << "Examples:\n"
              << "  " << programName << " --save \"buy groceries\"\n"
              << "  " << programName << " --save --ttl=2d \"buy groceries\"\n"
              << "  " << programName << " --read\n"
              << "  " << programName << " --help\n";
}