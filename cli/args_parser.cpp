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
        if (args.size() < 2) {
            result.errorMessage = "Missing text for --save";
            return result;
        }
        result.command = Save;
        result.text = args[1];
        if (args.size() > 2) {
            result.errorMessage = "Too many arguments for --save";
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
              << "  --save \"<text>\"   Store a text record on the server\n"
              << "  --read            Show all stored records\n"
              << "  --help            Display help information\n\n"
              << "Examples:\n"
              << "  " << programName << " --save \"buy groceries\"\n"
              << "  " << programName << " --read\n"
              << "  " << programName << " --help\n";
}