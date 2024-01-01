//
// Created by Bi Nguyen on 12/22/23.
//

#include "Parser.h"

std::vector<std::string> Parser::stringSplit(char delim) {
    std::vector<std::string> tokens;
    size_t startPos = 0;
    size_t curPos = usrInput.find(delim);
    while (curPos != std::string::npos){
        std::string token = usrInput.substr(startPos, curPos - startPos);
        tokens.emplace_back(token);
        startPos = curPos + 1;
        curPos = usrInput.find(usrInput, startPos);
    }
    std::string lastToken = usrInput.substr(startPos, std::string::npos);
    tokens.emplace_back(lastToken);
    return tokens;
}

Parser::ParsedCommand Parser::getParsedCommand() {
    std::vector<std::string> tokens = Parser::stringSplit(' ');
    ParsedCommand parsedInput;
    if (!tokens.empty()){
        //set command
        parsedInput.command = tokens.at(0);
        for(std::vector<std::string>::const_iterator const_it = tokens.begin() + 1; const_it != tokens.end(); ++const_it){
            if (const_it->find('-') != std::string::npos){
                //set options
                parsedInput.options.emplace_back(*const_it);
            }else{
                //set arguments
                parsedInput.arguments.emplace_back(*const_it);
            }
        }
    }
    return parsedInput;
}
