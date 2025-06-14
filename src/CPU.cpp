//
// Created by Dustin on 3/5/25.
//

#include "../include/CPU.h"

#include <chrono>
#include <cstring>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <thread>

#include "../include/Font.h"
#include "../include/OPCodes.h"

static constexpr int START_ADDRESS = 0x200;

CPUState::CPUState() {
    std::memset(registers_.data(), 0x0, sizeof(std::uint8_t) * REGISTER_COUNT);
    std::memcpy(memory_.data(), FONT_DATA.data(), sizeof(std::uint8_t) * FONT_DATA.size() );
    std::memset(memory_.data() + START_ADDRESS, 0x0, sizeof(std::byte) * (MEMORY_SIZE - START_ADDRESS));

    i_ = 0x0;
    pc_ = START_ADDRESS;
}

CPU::CPU(Display* d) : d_{d}, speed_{700} {}

void CPU::LoadRom(std::filesystem::path& path) {
    std::memset(state_.memory_.data(), 0x0, 0xFFF);

    std::ifstream rom{path};

    if(!rom.is_open())
        throw std::logic_error("Failed to load rom!");


    std::size_t read_amount = 0xFFF - 0x200;
    rom.read(reinterpret_cast<char *>(state_.memory_.data()), read_amount);
}

Instruction CPU::Fetch() {
    std::uint16_t byte1 = 0xFFF & static_cast<std::uint16_t>(state_.memory_[state_.pc_]);
    std::uint16_t byte2 = 0xFFF & (static_cast<std::uint16_t>(state_.memory_[state_.pc_ + 1]) << 8); // CHECK THIS

    state_.pc_ += 2;

    std::uint16_t op = byte1 | byte2;

    return Instruction{op};
}

void CPU::Execute(Instruction instruction) {
    std::stringstream ss;
    ss << std::hex << instruction.instruction_;
    std::string base_error = "Unsupported OP code: " + ss.str();

    // Replace this with a map from opcode to function pointer
    switch (instruction.nibble1_) {
        case 0x0:
            if (instruction.nibble3_ == 0xE) {
                if (instruction.nibble4_ == 0x0) {
                    OP_00E0();
                    return;
                } else if (instruction.nibble4_ == 0xE) {
                    OP_00EE();
                    return;
                }
            }
        case 0x1:
            OP_1NNN(instruction);
            return;
        case 0x2:
            OP_2NNN(instruction);
            return;
        case 0x3:
            OP_3XKK(instruction);
            return;
        case 0x4:
            OP_4XKK(instruction);
            return;
        case 0x5:
            OP_5XY0(instruction);
            return;
        case 0x6:
            OP_6XKK(instruction);
            return;
        case 0x7:
            OP_7XKK(instruction);
            return;
        case 0x8:
            if (instruction.nibble4_ == 0x1) {
                OP_8XY1(instruction);
                return;
            } else if (instruction.nibble4_ == 0x2) {
                OP_8XY2(instruction);
                return;
            } else if (instruction.nibble4_ == 0x3) {
                OP_8XY3(instruction);
                return;
            } else if (instruction.nibble4_ == 0x4) {
                OP_8XY4(instruction);
                return;
            } else if (instruction.nibble4_ == 0x5) {
                OP_8XY5(instruction);
                return;
            } else if (instruction.nibble4_ == 0x6) {
                OP_8XY6(instruction);
                return;
            } else if (instruction.nibble4_ == 0x7) {
                OP_8XY7(instruction);
                return;
            } else if (instruction.nibble4_ == 0xE) {
                OP_8XYE(instruction);
                return;
            }
        case 0x9:
            if (instruction.nibble4_ == 0x0) {
                OP_9XY0(instruction);
                return;
            }
        case 0xA:
            OP_ANNN(instruction);
            return;
        case 0xB:
            OP_BNNN(instruction);
            return;
        case 0xC:
            OP_CXKK(instruction);
            return;
        case 0xD:
            OP_DXYN(instruction);
            return;
        case 0xE:
            if (instruction.nibble3_ == 0x9 && instruction.nibble4_ == 0xE) {
                OP_EX9E(instruction);
                return;
            } else if (instruction.nibble3_ == 0xA && instruction.nibble4_ == 0x1) {
                OP_EXA1(instruction);
                return;
            }
        case 0xF:
            if (instruction.nibble3_ == 0x0 && instruction.nibble4_ == 0x7){
                OP_FX07(instruction);
                return;
            } else if (instruction.nibble3_ == 0x0 && instruction.nibble4_ == 0xA){
                OP_FX0A(instruction);
                return;
            } else if (instruction.nibble3_ == 0x1 && instruction.nibble4_ == 0x5){
                OP_FX15(instruction);
                return;
            } else if (instruction.nibble3_ == 0x1 && instruction.nibble4_ == 0x8){
                OP_FX18(instruction);
                return;
            } else if (instruction.nibble3_ == 0x1 && instruction.nibble4_ == 0xE){
                OP_FX1E(instruction);
                return;
            } else if (instruction.nibble3_ == 0x2 && instruction.nibble4_ == 0x9){
                OP_FX29(instruction);
                return;
            } else if (instruction.nibble3_ == 0x3 && instruction.nibble4_ == 0x3){
                OP_FX33(instruction);
                return;
            } else if (instruction.nibble3_ == 0x5 && instruction.nibble4_ == 0x5){
                OP_FX55(instruction);
                return;
            } else if (instruction.nibble3_ == 0x6 && instruction.nibble4_ == 0x5){
                OP_FX65(instruction);
                return;
            }
        default:
            throw std::logic_error(base_error);
    }
}

void CPU::Run() {

    // Time One second
    // Perform speed_ number of instructions
    // sleep for the rest
    auto start = std::chrono::steady_clock::now();

    for(int i = 0; i < speed_; i++) {
        auto instruction = Fetch();
        Execute(instruction);
    }


    auto end = std::chrono::steady_clock::now();

    auto cycle_duration = std::chrono::duration_cast<std::chrono::milliseconds>(start - end);
    auto sleep_time = std::chrono::milliseconds(1000) - cycle_duration;

    std::this_thread::sleep_for(sleep_time);
}

