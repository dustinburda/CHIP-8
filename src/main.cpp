#include "../include/CHIP8.h"


extern std::unordered_map<SDL_Keycode, std::uint8_t> KeyboardKey_KeypadKey = {
        {SDLK_1, 0x1},
        {SDLK_2, 0x2},
        {SDLK_3, 0x3},
        {SDLK_4, 0xC},
        {SDLK_q, 0x4},
        {SDLK_w, 0x5},
        {SDLK_e, 0x6},
        {SDLK_r, 0xD},
        {SDLK_a, 0x7},
        {SDLK_s, 0x8},
        {SDLK_d, 0x9},
        {SDLK_f, 0xE},
        {SDLK_z, 0xA},
        {SDLK_x, 0x0},
        {SDLK_c, 0xB},
        {SDLK_v, 0xF},
};


extern std::unordered_map<std::uint8_t, KeyState> KeypadKey_State {
        {0x0, KeyState::KeyUp},
        {0x1, KeyState::KeyUp},
        {0x2, KeyState::KeyUp},
        {0x3, KeyState::KeyUp},
        {0x4, KeyState::KeyUp},
        {0x5, KeyState::KeyUp},
        {0x6, KeyState::KeyUp},
        {0x7, KeyState::KeyUp},
        {0x8, KeyState::KeyUp},
        {0x9, KeyState::KeyUp},
        {0xA, KeyState::KeyUp},
        {0xB, KeyState::KeyUp},
        {0xC, KeyState::KeyUp},
        {0xD, KeyState::KeyUp},
        {0xE, KeyState::KeyUp},
        {0xF, KeyState::KeyUp}
};

int main(int argc, char** argv) {
    std::string rom_path = "../roms/pong2.ch8";

    if (argc == 2)
        rom_path = argv[1];
    else if (rom_path == "" )
        throw std::logic_error("Must only provide a single parameter: path to ROM file!");

    CHIP8 Chip8(Display::GetInstance());

    Chip8.LoadRom(rom_path);
    Chip8.Run();

    return 0;
}