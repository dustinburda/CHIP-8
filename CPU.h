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

struct Instruction {
    explicit Instruction(std::uint16_t instruction) : instruction_{instruction} {
        std::uint8_t mask = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);

        nibble1_ = instruction & mask;
        nibble2_ = (instruction & (mask << 4)) >> 4;
        nibble3_ = (instruction & (mask << 8)) >> 8;
        nibble4_ = (instruction & (mask << 12)) >> 12;
    }

    std::uint16_t instruction_;
    std::uint8_t nibble1_;
    std::uint8_t nibble2_;
    std::uint8_t nibble3_;
    std::uint8_t nibble4_;
};

struct CPUState {
    CPUState();

    std::array<std::uint8_t, REGISTER_COUNT> registers_;
    std::array<std::byte, MEMORY_SIZE> memory_;
    std::stack<std::uint16_t> stack_;

    std::uint16_t i_;
    std::uint16_t pc_;
};

class CPU {
public:
    CPU(Display* d);

    void LoadRom(std::filesystem::path& path);
    void Run();

private:
    Instruction Fetch();
    void Execute(Instruction instruction);

    void OP_00E0();
    void OP_00EE();
    void OP_1NNN(Instruction i);
    void OP_2NNN(Instruction i);
    void OP_3XKK(Instruction i);
    void OP_4XKK(Instruction i);
    void OP_5XY0(Instruction i);
    void OP_6XKK(Instruction i);
    void OP_7XKK(Instruction i);
    void OP_8XY1(Instruction i);
    void OP_8XY2(Instruction i);
    void OP_8XY3(Instruction i);
    void OP_8XY4(Instruction i);
    void OP_8XY5(Instruction i);
    void OP_8XY6(Instruction i);
    void OP_8XY7(Instruction i);
    void OP_8XYE(Instruction i);
    void OP_9XY0(Instruction i);
    void OP_ANNN(Instruction i);
    void OP_BNNN(Instruction i);
    void OP_CXKK(Instruction i);
    void OP_DXYN(Instruction i);
    void OP_EX9E(Instruction i);
    void OP_EXA1(Instruction i);

    CPUState state_;
    Display* d_;

    std::uint16_t speed_ {700};
};



#endif //CHIP_8_CPU_H
