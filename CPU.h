//
// Created by Dustin on 3/5/25.
//

#ifndef CHIP_8_CPU_H
#define CHIP_8_CPU_H

#include <array>
#include <cstdint>
#include <filesystem>


class CPU {
public:
    CPU() = default;

    void LoadRom(std::filesystem::path& path);
    void Fetch();
    void Decode();
    void Execute();

private:
    std::array<std::uint8_t, 16> registers_;
    std::array<std::byte, 4096> memory_;

    std::uint16_t stack_pointer_;
    std::uint16_t i_;
    std::uint16_t pc_;
};


#endif //CHIP_8_CPU_H
