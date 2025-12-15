#pragma once
#include <string>

enum CommandType {
    Help,
    Save,
    Read
};

struct ParseResult {
    CommandType command{CommandType::Help};
    std::string text;
    int ttlDays{7};
    std::string errorMessage;
    bool check() const { 
        return errorMessage.empty();
    }
};

class ArgsParser {
public:
    ParseResult parse(int argc, char* argv[]) const;
    static void printHelp(const std::string& programName);
};
