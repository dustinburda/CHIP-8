//
// Created by Dustin on 3/5/25.
//

#ifndef CHIP_8_CPU_H
#define CHIP_8_CPU_H

#include "Display.h"

#include <array>
#include <cstdint>
#include <filesystem>

static constexpr int REGISTER_COUNT = 16;
static constexpr int MEMORY_SIZE = 4096;
static constexpr int STACK_SIZE = 16;


struct CPUState {
    CPUState();

    std::array<std::uint8_t, REGISTER_COUNT> registers_;
    std::array<std::byte, MEMORY_SIZE> memory_;
    std::array<std::uint16_t, STACK_SIZE> stack_;

    std::uint16_t stack_pointer_;
    std::uint16_t i_;
    std::uint16_t pc_;
};

class CPU {
public:
    CPU(Display* d);

    void LoadRom(std::filesystem::path& path);
    void Run();

private:
    std::uint16_t Fetch();
    void Execute(std::uint16_t instruction);

    void OP_00E0();
    void OP_00EE();
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XKK();
    void OP_4XKK();
    void OP_5XY0();
    void OP_6XKK();
    void OP_7XKK();
    void OP_8XY1();
    void OP_8XY2();
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    void OP_8XYE();

    CPUState state_;
    Display* d_;

    std::uint16_t speed_ {700};
};



#endif //CHIP_8_CPU_H
