//
// Created by Bi Nguyen on 12/22/23.
//

#include "ShellInterface.h"


#define CTRL_C 3
#define NEWLINE '\n'
#define ESCAPE_CHAR '\x1b'
#define BACKSPACE 127
#define MOVE_LEFT_BY_1 "\x1b[D"
#define MOVE_RIGHT_BY_1 "\x1b[C"
#define CLEAR_LINE "\33[2K"


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
                history.addCommand(usrInput);
                if(usrInput == "quit"){
                    resetCanonicalMode();
                }
                usrInput.clear();
                displayPrompt();
                moveCursorToBeginningOfPrompt();

                break;

            default:
                handleInput(c);
        }
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
    writeSync(STDOUT_FILENO, cwd.c_str(), cwd.size());
}

void ShellInterface::writeSync(int fd,  const void* buf, int nbyte){
    write(fd, buf, nbyte);
//    fsync(STDOUT_FILENO);
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

    raw.c_lflag &= ~(ICANON | ECHO );

    //set minimum number character(s) for buffer
    raw.c_cc[VMIN] = 1;

    //set no timeout; allow no delay after user provide an input
    raw.c_cc[VTIME] = 0;

    //set attribute to terminal to take effect immediately (TCSANOW)
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


//when user press the up arrow key, if there's a set of input already. it sshould save it to the commandhistory
void ShellInterface::clearLine(){
    int numCharsInLine = usrInput.size() + promptLength;
    moveCursorToBeginningOfLine();
    for (int i = 0; i < numCharsInLine; i++){
        writeSync(STDOUT_FILENO, " ", 1);
    }
    moveCursorToBeginningOfLine();
}

void ShellInterface::updateScreen() {
    clearLine();
    displayPrompt();
    writeSync(STDOUT_FILENO, usrInput.c_str(), usrInput.size());
    moveCursorRightFromStart(promptLength + cursorInputPosition);
}

void ShellInterface::handleInsert(char c){

// when user add at the end of the line
    if (cursorInputPosition >= usrInput.size()){
        usrInput += c;
        writeSync(STDOUT_FILENO, &c, 1);
        ++cursorInputPosition;
    }else{
        //when user add in between characaters
        usrInput.insert(cursorInputPosition, 1, c);
        ++cursorInputPosition;
        updateScreen();
    }

// when user add the character in the middle of the line

}

void ShellInterface::handleBackspace() {
    if (isCursorInBounds(cursorInputPosition - 1)){
        usrInput.erase(cursorInputPosition - 1, 1);
        moveCursorLeft();
        writeSync(STDOUT_FILENO, " ", 1);
        writeSync(STDOUT_FILENO, MOVE_LEFT_BY_1, 3);
    }
}


void ShellInterface::handleInput(char c){
    handleInsert(c);
}

void ShellInterface::handleArrowKeys() {
    char sequence[2];
    read(STDIN_FILENO, &sequence[0], 1);
    read(STDIN_FILENO, &sequence[1], 1);
    if (sequence[0] == '[') {
        if (sequence[1] == 'A') {
            //Up Arrow
            history.showPreviousCommand();
            //write to stdout. add command to front of history
        } else if (sequence[1] == 'B') {
            //Down Arrow
            history.showNextCommand();
        } else if (sequence[1] == 'C') {
            //Right Arrow
            moveCursorRight();
        } else if (sequence[1] == 'D') {
            //Left Arrow
            moveCursorLeft();
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
        writeSync(STDOUT_FILENO, MOVE_LEFT_BY_1, 3);
    }
}

void ShellInterface::moveCursorRight(){
    if (isCursorInBounds(cursorInputPosition + 1)){
        ++cursorInputPosition;
        writeSync(STDOUT_FILENO, MOVE_RIGHT_BY_1, 3);
    }
}

void ShellInterface::moveCursorToBeginningOfLine(){
    writeSync(STDOUT_FILENO, "\r", 1);
}

void ShellInterface::moveCursorToBeginningOfPrompt() {
    moveCursorRightFromStart(promptLength);
}


void ShellInterface::moveCursorRightFromStart(int position) {
    moveCursorToBeginningOfLine();
    for (int i = 0; i < position; ++i){
        writeSync(STDOUT_FILENO, MOVE_RIGHT_BY_1, 3);
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


//TODO:
// 1. MOving cursor to the left should not allow them to move past $ or #
// 2. Moving cursor to the right ""
// 3. pressing up arrow, should save/add existing input to the history
// 4. add

//std::vector<std::string> ShellInterface::stringSplit(const std::string& s, char delimiter) {
//    std::vector<std::string> tokens;
//    std::stringstream stream(s);
//    std::string token;
//    //getline is useful when you want to read a line (or sequence of characters uptill specified charaacter)
//    while(std::getline(stream, token, delimiter)){
//        tokens.emplace_back(token);
//    }
//    return tokens;
//}
