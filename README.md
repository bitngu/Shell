# Interactive Shell Project

## Overview

This project is an interactive shell built in C++17 that serves as a platform for practicing system calls, interacting with the command line via `termios` library,
object-oriented programming (OOP) concepts and design,
and CMake for project build. The shell is designed to mimic the behavior of a real shell, providing users with a hands-on experience in a controlled environment.


## Features

- **Cursor Navigation:**
    - Use arrow keys (left, right) to move the cursor.
    - Utilize up and down arrow keys for browsing command history.

- **Text Manipulation:**
    - Backspace for character deletion.
    - Insertion between lines for efficient editing.

- **User Input History:**
    - The shell incorporates a double-linked list to efficiently track and manage user input history.

- **Supported Commands:**
    - `ls`: List directory contents
    - `cd`: Change directory
    - `pwd`: Print current working directory.
    - `ff`: Find a file in a specified directory
    - `>>`, `<<`, `>`, `<`: Redirections
    - `|`: Piping
    -  And more...


## Build and Run

To build the project, use CMake as follows:

```bash
  make
  
  ./build/Shell
```



## Cleanup
```bash
make clean
````

