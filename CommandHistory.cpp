//
// Created by Bi Nguyen on 12/24/23.
//

#include "CommandHistory.h"
#include "unistd.h"

void CommandHistory::addCommand(const std::string &command) {
    CommandNode* node = new CommandNode(command);
    if (currentCommand != nullptr){
        node->prevCommand = currentCommand;
        currentCommand->nextCommand = node;
    }
    currentCommand = node;
}

void CommandHistory::showPreviousCommand() {

    if (currentCommand != nullptr && currentCommand->prevCommand != nullptr){
        currentCommand = currentCommand->prevCommand;
        write(STDOUT_FILENO, &currentCommand->command, currentCommand->command.size());
    }
}

void CommandHistory::showNextCommand() {
    if (currentCommand != nullptr && currentCommand->nextCommand != nullptr){
        currentCommand = currentCommand->nextCommand;
        write(STDOUT_FILENO, &currentCommand->command, currentCommand->command.size());
    }
}
