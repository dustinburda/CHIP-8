//
// Created by Dustin on 3/5/25.
//

#include "../include/CHIP8.h"

#include <chrono>
#include <cstring>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <thread>

#include "../include/Font.h"

static constexpr int START_ADDRESS = 0x200;

CPU::CPU() {
    std::memset(registers_.data(), 0x0, sizeof(std::uint8_t) * REGISTER_COUNT);
    std::memcpy(memory_.data(), FONT_DATA.data(), sizeof(std::uint8_t) * FONT_DATA.size() );
    std::memset(memory_.data() + START_ADDRESS, 0x0, sizeof(std::byte) * (MEMORY_SIZE - START_ADDRESS));

    i_ = 0x0;
    pc_ = START_ADDRESS;
}

CHIP8::CHIP8(Display* d) : d_{d}, draw_flag{false} {}

void CHIP8::LoadRom(const std::filesystem::path& path) {
    std::memset(cpu_.memory_.data(), 0x0, 0xFFF);

    std::ifstream rom{path};

    if(!rom.is_open())
        throw std::logic_error("Failed to load rom!");


    std::size_t read_amount = 0xFFF - 0x200;
    rom.read(reinterpret_cast<char *>(cpu_.memory_.data() + START_ADDRESS), read_amount);
}

Instruction CHIP8::Fetch() {
    std::uint16_t byte1 = 0xFFFF & (static_cast<std::uint16_t>(cpu_.memory_[cpu_.pc_]) << 8);
    std::uint16_t byte2 = 0xFFFF & static_cast<std::uint16_t>(cpu_.memory_[cpu_.pc_ + 1]) ; // CHECK THIS

    cpu_.pc_ += 2;

    std::uint16_t op = byte1 | byte2;

    return Instruction{op};
}

void CHIP8::Execute(Instruction instruction) {
    std::stringstream ss;
    ss << std::hex << instruction.instruction_;
    std::string base_error = "Unsupported OP code: " + ss.str();

    std::cout << std::hex << instruction.instruction_ << std::endl;

    // Replace this with a map from opcode to function pointer
    switch (instruction.nibble4_) {
        case 0x0:
            if (instruction.nibble2_ == 0xE) {
                if (instruction.nibble1_ == 0x0) {
                    OP_00E0();
                    return;
                } else if (instruction.nibble1_ == 0xE) {
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
            if (instruction.nibble1_ == 0x0) {
                OP_8XY0(instruction);
                return;
            }
            if (instruction.nibble1_ == 0x1) {
                OP_8XY1(instruction);
                return;
            } else if (instruction.nibble1_ == 0x2) {
                OP_8XY2(instruction);
                return;
            } else if (instruction.nibble1_ == 0x3) {
                OP_8XY3(instruction);
                return;
            } else if (instruction.nibble1_ == 0x4) {
                OP_8XY4(instruction);
                return;
            } else if (instruction.nibble1_ == 0x5) {
                OP_8XY5(instruction);
                return;
            } else if (instruction.nibble1_ == 0x6) {
                OP_8XY6(instruction);
                return;
            } else if (instruction.nibble1_ == 0x7) {
                OP_8XY7(instruction);
                return;
            } else if (instruction.nibble1_ == 0xE) {
                OP_8XYE(instruction);
                return;
            }
        case 0x9:
            if (instruction.nibble1_ == 0x0) {
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
            if (instruction.nibble2_ == 0x9 && instruction.nibble1_ == 0xE) {
                OP_EX9E(instruction);
                return;
            } else if (instruction.nibble2_ == 0xA && instruction.nibble1_ == 0x1) {
                OP_EXA1(instruction);
                return;
            }
        case 0xF:
            if (instruction.nibble2_ == 0x0 && instruction.nibble1_ == 0x7){
                OP_FX07(instruction);
                return;
            } else if (instruction.nibble2_ == 0x0 && instruction.nibble1_ == 0xA){
                OP_FX0A(instruction);
                return;
            } else if (instruction.nibble2_ == 0x1 && instruction.nibble1_ == 0x5){
                OP_FX15(instruction);
                return;
            } else if (instruction.nibble2_ == 0x1 && instruction.nibble1_ == 0x8){
                OP_FX18(instruction);
                return;
            } else if (instruction.nibble2_ == 0x1 && instruction.nibble1_ == 0xE){
                OP_FX1E(instruction);
                return;
            } else if (instruction.nibble2_ == 0x2 && instruction.nibble1_ == 0x9){
                OP_FX29(instruction);
                return;
            } else if (instruction.nibble2_ == 0x3 && instruction.nibble1_ == 0x3){
                OP_FX33(instruction);
                return;
            } else if (instruction.nibble2_ == 0x5 && instruction.nibble1_ == 0x5){
                OP_FX55(instruction);
                return;
            } else if (instruction.nibble2_ == 0x6 && instruction.nibble1_ == 0x5){
                OP_FX65(instruction);
                return;
            }
        default:
            throw std::logic_error(base_error);
    }
}

void HandleInput(SDL_Event& event, bool& running) {
    auto SDL_Key = event.key.keysym.sym;

    if (event.type == SDL_QUIT) {
        std::cout << "Quitting";
        running = false;
    } else if (event.type == SDL_KEYDOWN) {
//        std::cout << KeypadKey_State.size() << std::endl;
//        for (auto [k, v] : KeypadKey_State)
//            std::cout << static_cast<int>(KeypadKey_State[k]) << "  " << static_cast<int>(k) << " " << static_cast<int>(v) << " " << static_cast<int>(KeypadKey_State[k]) << std::endl;
//        std::cout << std::endl;

        //  std::cout << SDL_GetKeyName(SDL_Key) << "  " << ((event.type == SDL_KEYDOWN) ? " Pressed" : " Released") << "\n";
        if (KeyboardKey_KeypadKey.count(SDL_Key) > 0) {
            std::uint8_t KeypadKey = KeyboardKey_KeypadKey[SDL_Key];
            KeypadKey_State[KeypadKey] =KeyState::KeyDown;
        }

//        std::cout << KeypadKey_State.size() << std::endl;
//        for (auto [k, v] : KeypadKey_State)
//            std::cout << static_cast<int>(KeypadKey_State[k]) << "  " << static_cast<int>(k) << " " << static_cast<int>(v) << " " << static_cast<int>(KeypadKey_State[k]) << std::endl;
//        std::cout << std::endl;

    } else if (event.type == SDL_KEYUP) {
        if (KeyboardKey_KeypadKey.count(SDL_Key) > 0) {
            std::uint8_t KeypadKey = KeyboardKey_KeypadKey[SDL_Key];
            KeypadKey_State[KeypadKey] = KeyState::KeyUp;
        }

//        std::cout << KeypadKey_State.size() << std::endl;
//        for (auto [k, v] : KeypadKey_State)
//            std::cout << static_cast<int>(KeypadKey_State[k]) << "  " << static_cast<int>(k) << " " << static_cast<int>(v) << " " << static_cast<int>(KeypadKey_State[k]) << std::endl;
//        std::cout << std::endl;
    }

}

void CHIP8::Run() {
    bool running = true;
    SDLWindow w;

    int cycle_count = 0;
    while(running) {
        auto start = std::chrono::steady_clock::now();

        auto instruction = Fetch();
        Execute(instruction);

//        std::cout << "Registers: " << std::endl;
//        for (auto & reg : cpu_.registers_) {
//            std::cout << std::hex << static_cast<int>(reg) << " ";
//        }
//        std::cout << std::endl;
//        std::cout << std::endl;

        SDL_Event event;
        while(SDL_PollEvent(&event))
            HandleInput(event, running);


    // Dump contents of buffer to screen

        // std::cout << "=======   " << refresh_count << "   =======" << std::endl;
        if (draw_flag) {
            draw_flag = false;
            w.RefreshDisplay(d_);
        }

        auto end = std::chrono::steady_clock::now();
        auto cycle_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        cycle_count++;

        if (cycle_count == 10) {
            cycle_count = 0;

            if (cpu_.dt_ > 0)
                cpu_.dt_--;
        }


    }
}

