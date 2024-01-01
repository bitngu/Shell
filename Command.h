//
// Created by Bi Nguyen on 12/24/23.
//

#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H


#include "string"
#include "Parser.h"

class Command {
public:
    std::string pwd();
    Parser::ParsedCommand parsedCommand;

    Command(const std::string& command,
            const std::vector<std::string>& options = {},
            const std::vector<std::string>& arguments = {})
            : parsedCommand{command, options, arguments} {};

    Command(const Parser::ParsedCommand& command): parsedCommand(command){};
private:
    //parser
};


#endif //SHELL_COMMAND_H
