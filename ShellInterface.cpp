//
// Created by Bi Nguyen on 12/22/23.
//

#include "ShellInterface.h"


#define CTRL_C 3
#define NEWLINE '\r'
#define ESCAPE_CHAR '\x1b'
#define BACKSPACE 127
#define MOVE_LEFT_BY_1 "\x1b[D"
#define MOVE_RIGHT_BY_1 "\x1b[C"
#define CLEAR_LINE "\33[2K"


//make clean && make && build/Shell

//current issue:
//unable to navigate back to original input
//- only able to do it for the first time. after that history is gone?

//maybe try adding it into the command?
//so when user press up, add the command into it. so they cana navigate back
//and if any enter key is pressed, we remove that value

//

[[noreturn]] void ShellInterface::execute() {
    sleep(1);
    setNonCanonicalMode();
    displayPrompt();


    while(true){
        char c;
        read(STDIN_FILENO, &c, sizeof(c));
        switch (c) {

            case CTRL_C:
                resetCanonicalMode();
                break;

            case ESCAPE_CHAR:
                handleArrowKeys();
                break;

            case BACKSPACE:
                handleBackspace();
                break;

            case NEWLINE:
                //parse commands;
                //run command
                handleEnter();
                break;

            default:
                handleInput(c);
        }
        updateScreen();
    }
}


void ShellInterface::displayPrompt() {
    std:: string cwd = cmd.pwd();
    int MAX_SIZE_DISPLAY = 16;
    // if path size is > 16, then we want to display it like /../dir_name

    if (cwd.size() > MAX_SIZE_DISPLAY){
        std::vector<std::string> tokens = stringSplit(cwd);
        std::string latestDirName = tokens[tokens.size() -1];
        cwd = "/../" + latestDirName;
    }
    promptLength = cwd.size();
    write(STDOUT_FILENO, cwd.c_str(), cwd.size());
}



void ShellInterface::resetCanonicalMode() {
    //set attribute to terminal to take effect immediately (TCSANOW)
    tcsetattr(STDIN_FILENO, TCSANOW, &savedAttributes);
    exit(0);
}

void ShellInterface::setNonCanonicalMode() {
    //save terminal attributes
    if(!isatty(STDOUT_FILENO)){
        fprintf (stderr, "Not a terminal.\n");
        exit(0);
    }

    tcgetattr(STDIN_FILENO, &savedAttributes);

    //set terminal to raw mode; disable CANONICAL and ECHO mode to handle user input
    struct termios raw;

    raw.c_lflag &= ~(ICANON | ECHO);

    //set minimum number character(s) for buffer
    raw.c_cc[VMIN] = 1;

    //set no timeout; allow no delay after user provide an input
    raw.c_cc[VTIME] = 0;

    //set attribute to terminal to take effect immediately (TCSANOW)
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


void ShellInterface::clearLine(){
    int numCharsInLine = usrInput.size() + promptLength;
    moveCursorToBeginningOfLine();
    for (int i = 0; i < numCharsInLine; i++){
        write(STDOUT_FILENO, " ", 1);
    }
    moveCursorToBeginningOfLine();
}

void ShellInterface::updateScreen() {
    clearLine();
    displayPrompt();
    write(STDOUT_FILENO, usrInput.c_str(), usrInput.size());
    moveCursorRightFromStart(promptLength + cursorInputPosition);
}


void ShellInterface::handleEnter(){
    if (!usrInput.empty()){
        //update state
        history.addCommand(usrInput);
        //executeCommand;
        usrInput.clear();
        isActiveInput = true;
        delete historyCopy;
        historyCopy = nullptr;
        history.currentCommand = nullptr;
        cursorInputPosition = 0;
    }
    write(STDOUT_FILENO, "\n", 1);
}

void ShellInterface::handleBackspace() {

    if (isCursorInBounds(cursorInputPosition - 1)){
        //In the situation where users want to remove char in between current input
        //when updateScreen gets called, we already decrement the position
        //which cause a character in the line to be not completely cleared
        //therefore, need to clear line here while we have the original size
        int currentPosition = cursorInputPosition;
        clearLine();
        usrInput.erase(cursorInputPosition - 1, 1);
        cursorInputPosition = currentPosition - 1;
    }
}

//say they are perofrm:
//abc
//def
//g <- didn't press enter, goes up

//at def <- make some modification for example adding hi -> defhi, need to update the command
//but if they press enter. we don't want to update the command. we add it to the new command
//so when
//this applies for all of them

//i think i need to create a copy of it.
//for exaample


//abc

//hi

//g


//then i press up so at "hi" modified to add "t" => "hit", don't press enter
//go up one more, at "abc", modify it. delete "bc" => "a"
//go back down to "g". modify it. add k. so it's "gk" and then press enter
//now when i go back to history, i get gk, hi, abc instead of gk, a, hit
//

//so I probably need a history copy.
//when first time i press up key. store the state of history
//
//user can do whatever. modify different state. it'll get updated ad such from the copy history
//but once they press enter. we just add the new command to the history
//clear the copy history
//right now, i have everything set to modify the/update using the original
//

void ShellInterface::handleInput(char c){
    //need to update the command
    if (cursorInputPosition >= usrInput.size()) {
        // add at the end of the line
        usrInput += c;
        if (historyCopy && historyCopy->currentCommand != nullptr){
            historyCopy->currentCommand->command += c;
        }
    }
    else {
        // add between the line
        usrInput.insert(cursorInputPosition, 1, c);
        if (historyCopy && historyCopy->currentCommand != nullptr){
            historyCopy->currentCommand->command.insert(cursorInputPosition, 1, c);
        }
    }

    ++cursorInputPosition;
}

void ShellInterface::handleArrowKeys() {
    char sequence[2];
    read(STDIN_FILENO, &sequence[0], 1);
    read(STDIN_FILENO, &sequence[1], 1);

    if (sequence[0] == '[') {
        if (sequence[1] == 'A') {
            //Up Arrow
            //next command in linked list

            //if there is history to be stored
            if (isActiveInput && history.headCommand != nullptr) {
                //save original input before navigating through history
                historyCopy = new CommandHistory(history);
                isActiveInput = false;
            }

            if (historyCopy) {
                clearLine();
                if (historyCopy->currentCommand == nullptr) {
                    //store input that user wrote to before the up arrow
                    historyCopy->currentCommand = historyCopy->headCommand;
                    historyCopy->addCommand(usrInput);
                    usrInput = historyCopy->currentCommand->command;
                    cursorInputPosition = static_cast<int> (usrInput.size());
                } else if (historyCopy->currentCommand->nextCommand != nullptr) {
                    historyCopy->currentCommand = historyCopy->getNextCommand();
                    usrInput = historyCopy->currentCommand->command;
                    cursorInputPosition = static_cast<int> (usrInput.size());
                }
            }
        }

        else if (sequence[1] == 'B') {
            //Down Arrow
            //prev command in linked list

            if (historyCopy) {
                clearLine();
                //get prev (in front) command
                if (historyCopy->currentCommand->prevCommand != nullptr) {
                    historyCopy->currentCommand = historyCopy->getPreviousCommand();
                    usrInput = historyCopy->currentCommand->command;
                    cursorInputPosition = static_cast<int> (usrInput.size());
                } else {
                    //get original input
//                    usrInput = historyCopy->currentCommand->command;
                    usrInput = historyCopy->headCommand->command;
                    cursorInputPosition = static_cast<int> (usrInput.size());
                }
            }

        } else if (sequence[1] == 'C') {
            //Right Arrow
            if (isCursorInBounds(cursorInputPosition + 1)) ++cursorInputPosition;
        }
        else if (sequence[1] == 'D') {
            //Left Arrow
            if (isCursorInBounds(cursorInputPosition - 1)) --cursorInputPosition;
        }
    }
}


bool ShellInterface::isCursorInBounds(int pos) {
    bool isWithinLeftBound = promptLength + pos >= promptLength;
    bool isWithinRightBound = pos <= usrInput.size();
    return isWithinLeftBound && isWithinRightBound;
}


void ShellInterface::moveCursorLeft(){
    if (isCursorInBounds(cursorInputPosition - 1)){
        --cursorInputPosition;
        write(STDOUT_FILENO, MOVE_LEFT_BY_1, 3);
    }
}

void ShellInterface::moveCursorRight(){
    if (isCursorInBounds(cursorInputPosition + 1)){
        ++cursorInputPosition;
        write(STDOUT_FILENO, MOVE_RIGHT_BY_1, 3);
    }
}

void ShellInterface::moveCursorToBeginningOfLine(){
    write(STDOUT_FILENO, "\r", 1);
}

void ShellInterface::moveCursorToBeginningOfPrompt() {
    moveCursorRightFromStart(promptLength);
}


void ShellInterface::moveCursorRightFromStart(int position) {
    moveCursorToBeginningOfLine();
    for (int i = 0; i < position; ++i){
        write(STDOUT_FILENO, MOVE_RIGHT_BY_1, 3);
    }
}

void ShellInterface::moveCursorToEndOfLine(){
    cursorInputPosition = static_cast<int> (usrInput.size()) + 1;
    moveCursorRightFromStart(cursorInputPosition);
}

std::vector<std::string> ShellInterface::stringSplit(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    size_t startPos = 0;
    size_t curPos = s.find(delimiter);
    while (curPos != std::string::npos){
        std::string token = s.substr(startPos, curPos);
        tokens.emplace_back(token);
        startPos = curPos;
        curPos = s.find(delimiter);
    }
    return tokens;
}


