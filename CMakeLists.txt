cmake_minimum_required(VERSION 3.26)
project(Shell)

set(CMAKE_CXX_STANDARD 17)

add_executable(Shell main.cpp
        Shell.cpp
        Shell.h
        ShellInterface.cpp
        ShellInterface.h
        Parser.cpp
        Parser.h
        CommandNode.h
        CommandHistory.cpp
        CommandHistory.h
        Command.cpp
        Command.h
)



