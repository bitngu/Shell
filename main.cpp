


#include "ShellInterface.h"

int main(int argc, char** argv) {

    ShellInterface aShell = ShellInterface();
    aShell.execute();
    aShell.resetCanonicalMode();
}