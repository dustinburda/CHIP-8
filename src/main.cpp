#include "../include/CPU.h"

int main(int argc, char** argv) {
    std::string rom_path = "../roms/chip8-splash.ch8";

    if (argc == 2) {
        rom_path = argv[1];
    } else if (rom_path == "" ) {
        throw std::logic_error("Must only provide a single path parameter to ROM file!");
    }

    CPU Chip8(Display::GetInstance());

    Chip8.LoadRom(rom_path);
    Chip8.Run();

    return 0;
}
