//
// Created by Dustin on 3/30/25.
//

#ifndef CHIP_8_WINDOW_H
#define CHIP_8_WINDOW_H

#include <SDL2/SDL.h>
#include <iostream>

class SDLWindow {
public:
    SDLWindow() {
        const int SCREEN_WIDTH = 640;
        const int SCREEN_HEIGHT = 320;

        /* Initialise SDL */
        if( SDL_Init( SDL_INIT_VIDEO ) < 0){
            std::cerr << "Could not initialise SDL: " << SDL_GetError()  << "\n";
            throw std::logic_error("Failed to initialize SDL");
        }

        /* Set a video mode */
        window_ = SDL_CreateWindow(
                "SDL Window",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
        );
    }

    ~SDLWindow() {
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }
private:
    SDL_Window* window_{nullptr};
};


#endif //CHIP_8_WINDOW_H
