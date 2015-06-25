#include <iostream>
#include <cstdlib>
#include "gameboy.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " filename" << std::endl;
        return EXIT_FAILURE;
    }

    GameBoy gb;
    if (!gb.loadRom(argv[1])) {
        return EXIT_FAILURE;
    }

    gb.initialize();

    return EXIT_SUCCESS;
}
