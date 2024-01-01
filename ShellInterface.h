//
// Created by Bi Nguyen on 12/22/23.
//

#ifndef SHELL_SHELLINTERFACE_H
#define SHELL_SHELLINTERFACE_H


#include "CommandHistory.h"
#include "Command.h"
#include "Parser.h"
#include "sstream"
#include "termios.h"
#include "unistd.h"

//debug
#include "fstream"

class ShellInterface {

public:
    void resetCanonicalMode();

    [[noreturn]] void execute();


private:
    CommandHistory history;
    CommandHistory* historyCopy = nullptr;
    std::ofstream logger{"shell.txt"};
    std:: string usrInput{};
    bool isActiveInput = true;
    int cursorInputPosition = 0;

    int promptLength = 0;
    void displayPrompt();
    bool isCursorInBounds(int pos);
    void handleInput(char c);
    void handleArrowKeys();
    void handleBackspace();
    void handleEnter();

    static std::vector<std::string> stringSplit(const std::string& s, char delimiter = '/');

    void updateScreen();

    void clearLine();
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorToBeginningOfLine();
    void moveCursorToBeginningOfPrompt();
    void moveCursorRightFromStart(int position);
    void moveCursorToEndOfLine();

    //state of terminal
    struct termios savedAttributes;

    //default state of terminal => buffer til newline
//    void resetCanonicalMode();

    //raw mode of terminal => detect character input
    void setNonCanonicalMode();

//    ~ShellInterface()

};


#endif //SHELL_SHELLINTERFACE_H
