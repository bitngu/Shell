//
// Created by Bi Nguyen on 12/24/23.
//

#ifndef SHELL_COMMANDNODE_H
#define SHELL_COMMANDNODE_H

#include "string"

class CommandNode {
public:
    std::string command;
    CommandNode* prevCommand;
    CommandNode* nextCommand;
    explicit CommandNode(std::string command): command(command), prevCommand(nullptr), nextCommand(nullptr) {};
};


#endif //SHELL_COMMANDNODE_H
