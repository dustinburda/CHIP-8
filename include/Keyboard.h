//
// Created by Dustin on 3/30/25.
//

#ifndef CHIP_8_KEYBOARD_H
#define CHIP_8_KEYBOARD_H

#include <SDL2/SDL.h>

#include <unordered_map>




static std::unordered_map<SDL_Keycode, std::uint8_t> KeyboardKey_KeypadKey = {
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

enum class KeyState {
    KeyDown,
    KeyUp
};

static std::unordered_map<std::uint8_t, KeyState> KeypadKey_State {
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

#endif //CHIP_8_KEYBOARD_H
