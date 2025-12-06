#include "args_parser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    const std::string programName = (argc > 0 && argv[0] != nullptr) ? argv[0] : "./cli/build/stu";

    ArgsParser parser;
    const ParseResult parsed = parser.parse(argc, argv);

    if (!parsed.check()) {
        std::cerr << parsed.errorMessage << "\n\n";
        ArgsParser::printHelp(programName);
        return 1;
    }
    if (parsed.command == Help) {
        ArgsParser::printHelp(programName);
        return 0;
    } else if (parsed.command == Save) {
        std::cout << "Save command received. Text: " << parsed.text << "\n";
        return 0;
    } else if (parsed.command == Read) {
        std::cout << "Read command received\n";
        return 0;
    }

    return 1;
}
