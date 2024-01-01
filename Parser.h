//
// Created by Bi Nguyen on 12/22/23.
//

#ifndef SHELL_PARSER_H
#define SHELL_PARSER_H

#include "vector"
#include "string"

class Parser {
public:
    std::string usrInput;

    struct ParsedCommand {
        std::string command;
        std::vector<std::string> options;
        std::vector<std::string> arguments;
    };
    explicit Parser(const std::string& input): usrInput(input){};
    ParsedCommand getParsedCommand();


private:
    std::vector<std::string> stringSplit(char delim);


};


#endif //SHELL_PARSER_H
