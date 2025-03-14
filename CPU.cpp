//
// Created by Dustin on 3/5/25.
//

#include "CPU.h"

#include "OPCodes.h"

#include <chrono>
#include <cstring>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <thread>

static constexpr int START_ADDRESS = 0x200;

CPUState::CPUState() {
    std::memset(registers_.data(), 0x0, sizeof(std::uint8_t) * REGISTER_COUNT);
    std::memset(memory_.data(), 0x0, sizeof(std::byte) * MEMORY_SIZE);
    std::memset(stack_.data(), 0x0, sizeof (std::uint16_t) * STACK_SIZE);
    stack_pointer_ = 0x0;
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
            OP_1NNN();
            return;
        case 0x2:
            OP_2NNN();
            return;
        case 0x3:
            OP_3XKK();
            return;
        case 0x4:
            OP_4XKK();
            return;
        case 0x5:
            OP_5XY0();
            return;
        case 0x6:
            OP_6XKK();
            return;
        case 0x7:
            OP_7XKK();
            return;
        case 0x8:
            if (instruction.nibble4_ == 0x1) {
                OP_8XY1();
                return;
            } else if (instruction.nibble4_ == 0x2) {
                OP_8XY2();
                return;
            } else if (instruction.nibble4_ == 0x3) {
                OP_8XY3();
                return;
            } else if (instruction.nibble4_ == 0x4) {
                OP_8XY4();
                return;
            } else if (instruction.nibble4_ == 0x5) {
                OP_8XY5();
                return;
            } else if (instruction.nibble4_ == 0x6) {
                OP_8XY6();
                return;
            } else if (instruction.nibble4_ == 0x7) {
                OP_8XY7();
                return;
            } else if (instruction.nibble4_ == 0xE) {
                OP_8XYE();
                return;
            }
        case 0x9:
            if (instruction.nibble4_ == 0x0) {
                OP_9XY0();
                return;
            }
        case 0xA:
            OP_ANNN();
            return;
        case 0xB:
            OP_BNNN();
            return;
        case 0xC:
            OP_CXKK();
            return;
        case 0xD:
            OP_DXYN();
            return;
        case 0xE:
            if (instruction.nibble3_ == 0x9 && instruction.nibble4_ == 0xE) {
                OP_EX9E();
                return;
            } else if (instruction.nibble3_ == 0xA && instruction.nibble4_ == 0x1) {
                OP_EXA1();
                return;
            }
        case 0xF:
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

