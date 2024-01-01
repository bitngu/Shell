//
// Created by Bi Nguyen on 12/24/23.
//

#include "Command.h"

std::string Command::pwd() {
    std::string test = "";
    if (!parsedCommand.command.empty()){
        test = "/../Shell: #";
    }
    return test;
}
