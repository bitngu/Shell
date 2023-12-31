//
// Created by Bi Nguyen on 12/24/23.
//

#include "CommandHistory.h"
#include "unistd.h"

//head> 1 <tail
// add 2
// 2-> 1
//  2 <- 1
void CommandHistory::addCommand(const std::string &command) {
    CommandNode* node = new CommandNode(command);

    if (headCommand == nullptr){
        headCommand = node;
        tailCommand = node;
    }
    else{
        node->nextCommand = headCommand;
        headCommand->prevCommand = node;
        headCommand = node;
    }
//    printDebug();
}


void CommandHistory::removeTopCommand() {
    if (headCommand != nullptr){
        CommandNode* newHead = headCommand->nextCommand;
        if (newHead != nullptr){
            newHead->prevCommand = nullptr;
        }
        delete headCommand;
        headCommand = newHead;
    }
}

CommandNode* CommandHistory::getPreviousCommand() {
    if (currentCommand != nullptr){
        currentCommand = currentCommand->prevCommand;
    }
    return currentCommand;
}

CommandNode* CommandHistory::getNextCommand() {
    if (currentCommand != nullptr){
        currentCommand = currentCommand->nextCommand;
    }
    return currentCommand;
}


void CommandHistory::printDebug2(){
    CommandNode* currentNode = currentCommand;
    while(currentNode != nullptr){
        logfile << currentNode->command << "->";
        currentNode = currentNode->nextCommand;
        logfile.flush();
    }
    logfile << "\n";
};
void CommandHistory::printDebug() {
    CommandNode* currentNode = headCommand;
    while(currentNode != nullptr){
        logfile << currentNode->command << "->";
        currentNode = currentNode->nextCommand;
        logfile.flush();
    }
    logfile << "\n";
}


CommandHistory::~CommandHistory() {
    CommandNode* currentNode = tailCommand;
    while (currentNode != nullptr){
        CommandNode* tmp = currentNode->prevCommand;
        delete currentNode;
        currentNode = tmp;
    }
}

CommandHistory::CommandHistory(const CommandHistory &other) {
    CommandNode* currentNode = other.tailCommand;
    while(currentNode != nullptr){
        addCommand(currentNode->command);
        currentNode = currentNode->prevCommand;
    }
}
