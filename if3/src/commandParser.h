#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <algorithm>
#include <vector>

class CommandParser {
public:
    static std::string toLower(const std::string& str);
    static std::vector<std::string> split(const std::string& s, char delimiter);
};

#endif // COMMAND_PARSER_H
