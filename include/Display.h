//
// Created by Dustin on 3/5/25.
//

#ifndef CHIP_8_DISPLAY_H
#define CHIP_8_DISPLAY_H

#include <array>
#include <cstdint>

static constexpr int HEIGHT = 32;
static constexpr int WIDTH = 64;

using Color = std::array<std::uint8_t, 3>;

class Display {
public:
    static Display* GetInstance() {
        static Display* d = new Display();
        return d;
    }

    void ClearDisplay() {
        std::memset(buffer_.data()->data(), 0x0, sizeof (uint8_t) * (WIDTH * HEIGHT));
    }

    void GetRGBBuffer(std::array<std::array<Color , WIDTH>, HEIGHT>& buffer);
    const std::array<std::array<uint8_t , WIDTH>, HEIGHT>& GetBuffer() const;
    void Set(int y, int x, uint8_t byte);
    uint8_t Get(int y, int x);


private:
    explicit Display() {
        std::memset(buffer_.data(), 0x0, sizeof (uint8_t) * WIDTH * HEIGHT);
    }

    std::array<std::array<uint8_t , WIDTH>, HEIGHT> buffer_;
};


#endif //CHIP_8_DISPLAY_H
