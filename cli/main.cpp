#include "args_parser.hpp"
#include "commands.hpp"
#include "http_client.hpp"
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
    }

    HttpClient client;

    if (parsed.command == Save) {
        return commandSave(parsed, client);
    } else if (parsed.command == Read) {
        return commandRead(parsed, client);
    }

    return 1;
}