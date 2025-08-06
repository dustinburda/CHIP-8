//
// Created by Dustin on 3/5/25.
//

#include "../include/Display.h"

const std::array<std::array<uint8_t, WIDTH>, HEIGHT>& Display::GetBuffer() const {
    return buffer_;
}

void Display::Set(int y, int x, uint8_t byte) {
    if (y >= HEIGHT || y < 0 || x >= WIDTH || x < 0)
        throw std::logic_error("Pixel dimensions must be within display dimension!");

    buffer_[y][x] = byte;
}

uint8_t Display::Get(int y, int x) {
    if (y >= HEIGHT || y < 0 || x >= WIDTH || x < 0)
        throw std::logic_error("Pixel dimensions must be within display dimension!");

    return buffer_[y][x];
}