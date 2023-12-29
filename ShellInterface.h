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
    std:: string usrInput;
    int cursorInputPosition = 0;
    CommandHistory history = CommandHistory();
    Command cmd = Command();

    int promptLength = 0;
    void displayPrompt();
    void writeSync(int fd,  const void *buf, int nbyte);
    bool isCursorInBounds(int pos);
    void handleInput(char c);
    void handleArrowKeys();
    void handleInsert(char c);
    void handleBackspace();

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

};


#endif //SHELL_SHELLINTERFACE_H
