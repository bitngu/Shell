//
// Created by Bi Nguyen on 12/24/23.
//

#ifndef SHELL_COMMANDHISTORY_H
#define SHELL_COMMANDHISTORY_H

#include "fstream"

#include "CommandNode.h"
class CommandHistory {
public:
    std::ofstream logfile{"history.txt"};
    void printDebug();
    void printDebug2();
    CommandNode* headCommand;
    CommandNode* tailCommand;
    CommandNode* currentCommand;

    void addCommand(const std::string& command);
    void removeTopCommand();
    CommandNode* getPreviousCommand();
    CommandNode* getNextCommand();
    CommandHistory(): headCommand(nullptr), tailCommand(nullptr), currentCommand(nullptr){};
    CommandHistory(const CommandHistory &other);
    ~CommandHistory();

};


#endif //SHELL_COMMANDHISTORY_H
