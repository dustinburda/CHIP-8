#include <iostream>

#include <SDL2/SDL.h>

#include "Keyboard.h"
#include "Window.h"

int main() {

    SDL_Event event;
    bool running = true;

    SDLWindow w;

    while(running) {
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Quitting";
                running = false;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                auto SDL_Key = event.key.keysym.sym;

                //  std::cout << SDL_GetKeyName(SDL_Key) << "  " << ((event.type == SDL_KEYDOWN) ? " Pressed" : " Released") << "\n";
                if (KeyboardKey_KeypadKey.count(SDL_Key) > 0) {
                    auto KeypadKey = KeyboardKey_KeypadKey[SDL_Key];
                    KeypadKey_State[KeypadKey] = (event.type == SDL_KEYDOWN) ? KeyState::KeyDown
                                                                             : KeyState::KeyUp;
                }
            }
        }
    }

    return 0;
}
