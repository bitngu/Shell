//
// Created by Bi Nguyen on 12/24/23.
//

#ifndef SHELL_COMMANDHISTORY_H
#define SHELL_COMMANDHISTORY_H


#include "CommandNode.h"
class CommandHistory {
public:
    CommandNode* currentCommand;
    void addCommand(const std::string& command);
    void showPreviousCommand();
    void showNextCommand();
    CommandHistory(): currentCommand(nullptr){};

};


#endif //SHELL_COMMANDHISTORY_H
