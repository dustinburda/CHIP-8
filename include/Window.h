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
        const int SCREEN_WIDTH = 1280;
        const int SCREEN_HEIGHT = 640;

        /* Initialise SDL */
        if( SDL_Init( SDL_INIT_EVERYTHING ) < 0){
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

        r_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
        SDL_RenderSetLogicalSize(r_, WIDTH, HEIGHT);

        t_ = SDL_CreateTexture(r_, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    }

    void RefreshDisplay(Display* d) {
        uint32_t buffer[64 * 32];

        auto& d_buffer = d->GetBuffer();
        for (int i = 0; i < HEIGHT; i++) {
            for(int j = 0; j < WIDTH; j++) {
                buffer[i * 64 + j] = (d_buffer[i][j] == 0x1) ? 0xFFFFFFFF : 0x00000000;
            }
        }

        SDL_UpdateTexture(t_, nullptr, buffer, WIDTH * sizeof(uint32_t));
        SDL_RenderClear(r_);
        SDL_RenderCopy(r_, t_, NULL, NULL);
        SDL_RenderPresent(r_);
    }


    ~SDLWindow() {
        SDL_DestroyWindow(window_);
        SDL_DestroyRenderer(r_);
        SDL_DestroyTexture(t_);
        SDL_Quit();
    }
private:
    SDL_Window* window_{nullptr};
    SDL_Renderer* r_ {nullptr};
    SDL_Texture* t_{nullptr};

};


#endif //CHIP_8_WINDOW_H
