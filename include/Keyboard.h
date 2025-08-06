//
// Created by Dustin on 3/30/25.
//

#ifndef CHIP_8_KEYBOARD_H
#define CHIP_8_KEYBOARD_H

#include <SDL2/SDL.h>

#include <unordered_map>




extern std::unordered_map<SDL_Keycode, std::uint8_t> KeyboardKey_KeypadKey;

enum class KeyState {
    KeyDown,
    KeyUp
};

extern std::unordered_map<std::uint8_t, KeyState> KeypadKey_State;

#endif //CHIP_8_KEYBOARD_H
