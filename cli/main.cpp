#include <iostream>
#include <string>
#include <vector>

namespace {

void printHelp(const std::string &programName) {
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

bool hasHelpFlag(const std::vector<std::string> &args) {
    for (const auto &arg : args) {
        if (arg == "--help") {
            return true;
        }
    }
    return false;
}

}

int main(int argc, char *argv[]) {
    const std::string programName = (argc > 0 && argv[0] != nullptr) ? argv[0] : "./cli/build/stu";

    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    if (args.empty() || hasHelpFlag(args)) {
        printHelp(programName);
        return 0;
    }

    std::cerr << "Unknown command. Use --help to see available commands\n";
    return 1;
}
