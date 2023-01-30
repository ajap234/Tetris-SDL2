// For consoleless app, in to solution properties, go to Linker, then System, and set Subsystem to Windows (/SUBSYSTEM:WINDOWS) 

#include <iostream>

#include "game.hpp"

int main(int argc, char* argv[])
{    
    Game tetris;
    
    if (!tetris.init())
    {
        return EXIT_FAILURE;
    }

    tetris.run();

    return EXIT_SUCCESS;
}
