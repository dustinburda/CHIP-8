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

std::uint16_t CPU::Fetch() {
    std::uint16_t nibble1 = 0xFFF & static_cast<std::uint16_t>(state_.memory_[state_.pc_]);
    std::uint16_t nibble2 = 0xFFF & (static_cast<std::uint16_t>(state_.memory_[state_.pc_ + 1]) << 8); // CHECK THIS

    state_.pc_ += 2;

    std::uint16_t op = nibble1 | nibble2;

    return op;
}

void CPU::Execute(std::uint16_t instruction) {
    std::uint8_t mask = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);

    std::uint8_t nibble1 = instruction & mask;
    std::uint8_t nibble2 = (instruction & (mask << 4)) >> 4;
    std::uint8_t nibble3 = (instruction & (mask << 8)) >> 8;
    std::uint8_t nibble4 = (instruction & (mask << 12)) >> 12;

    std::stringstream ss;
    ss << std::hex << instruction;
    std::string base_error = "Unsupported OP code: " + ss.str();

    // Replace this with a map from opcode to function pointer
    switch (nibble1) {
        case 0x0:
            if (nibble3 == 0xE) {
                if (nibble4 == 0x0) {
                    OP_00E0();
                } else if (nibble4 == 0xE) {
                    OP_00EE();
                } else
                    throw std::logic_error(base_error);
            }
        case 0x1:

        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
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
        std::uint16_t instruction = Fetch();
        Execute(instruction);
    }


    auto end = std::chrono::steady_clock::now();

    auto cycle_duration = std::chrono::duration_cast<std::chrono::milliseconds>(start - end);
    auto sleep_time = std::chrono::milliseconds(1000) - cycle_duration;

    std::this_thread::sleep_for(sleep_time);
}

