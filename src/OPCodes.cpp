//
// Created by Dustin on 3/6/25.
//

#ifndef CHIP_8_OPCODES_CPP
#define CHIP_8_OPCODES_CPP

#include "../include/util.h"

#include "../include/CHIP8.h"
#include "../include/Font.h"

void CHIP8::OP_00E0() {
    d_->ClearDisplay();
    draw_flag = true;
}

void CHIP8::OP_00EE() {
    cpu_.pc_ = cpu_.stack_.top();
    cpu_.stack_.pop();
}

void CHIP8::OP_1NNN(Instruction i) {
    std::uint16_t NNN = (i.instruction_ & 0xFFF);
    cpu_.pc_ = NNN;
}

void CHIP8::OP_2NNN(Instruction i) {
    cpu_.stack_.push(cpu_.pc_);

    std::uint16_t NNN = (i.instruction_ & 0xFFF);
    cpu_.pc_ = NNN;

}

void CHIP8::OP_3XKK(Instruction i) {
    uint8_t X = i.nibble3_;

    uint8_t KK = (i.nibble2_ << 4) | i.nibble1_;

    if (cpu_.registers_[X] == KK)
        cpu_.pc_ += 2;
}

void CHIP8::OP_4XKK(Instruction i) {
    uint8_t X = i.nibble3_;

    uint8_t KK = (i.nibble2_ << 4) | i.nibble1_;

    if (cpu_.registers_[X] != KK)
        cpu_.pc_ += 2;
}

void CHIP8::OP_5XY0(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    if(cpu_.registers_[X] == cpu_.registers_[Y])
        cpu_.pc_ += 2;
}

void CHIP8::OP_6XKK(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t KK = (i.nibble2_ << 4) | i.nibble1_;

    cpu_.registers_[X] = KK;
}

void CHIP8::OP_7XKK(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t KK = (i.nibble2_ << 4) | i.nibble1_;

    cpu_.registers_[X] += KK;
}

void CHIP8::OP_8XY0(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    cpu_.registers_[X] = cpu_.registers_[Y];
}

void CHIP8::OP_8XY1(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    cpu_.registers_[X] = (cpu_.registers_[X] | cpu_.registers_[Y]);
}

void CHIP8::OP_8XY2(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    cpu_.registers_[X] = (cpu_.registers_[X] & cpu_.registers_[Y]);
}

void CHIP8::OP_8XY3(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    cpu_.registers_[X] = (cpu_.registers_[X] ^ cpu_.registers_[Y]);
}

void CHIP8::OP_8XY4(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    auto reg_x = cpu_.registers_[X];
    auto reg_y = cpu_.registers_[Y];

    cpu_.registers_[X] = reg_x + reg_y;
    cpu_.registers_[0xF] = (reg_x > 255 - reg_y);
}

void CHIP8::OP_8XY5(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    auto reg_x = cpu_.registers_[X];
    auto reg_y = cpu_.registers_[Y];

    // TODO: GEQ or EQ?
    cpu_.registers_[X] = reg_x - reg_y;
    cpu_.registers_[0xF] = (reg_x >= reg_y);
}

void CHIP8::OP_8XY6(Instruction i) {
    uint8_t X = i.nibble3_;

    uint8_t LSB = cpu_.registers_[X] & 0x1;
    cpu_.registers_[X] >>= 1;
    cpu_.registers_[0xF] = LSB;
}

void CHIP8::OP_8XY7(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    auto reg_x = cpu_.registers_[X];
    auto reg_y = cpu_.registers_[Y];

    // TODO: GEQ or EQ?
    cpu_.registers_[X] = reg_y - reg_x;
    cpu_.registers_[0xF] = (reg_y >= reg_x);
}

void CHIP8::OP_8XYE(Instruction i) {
    uint8_t X = i.nibble3_;

    uint8_t MSB = (cpu_.registers_[X] & (0x1 << 7)) >> 7;
    cpu_.registers_[X] <<= 1;
    cpu_.registers_[0xF] = MSB;
}

void CHIP8::OP_9XY0(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    if(cpu_.registers_[X] != cpu_.registers_[Y])
        cpu_.pc_ += 2;
}

void CHIP8::OP_ANNN(Instruction i) {
    std::uint16_t NNN = (i.instruction_ & 0xFFF);
    cpu_.i_ = NNN;
}

void CHIP8::OP_BNNN(Instruction i) {
    std::uint16_t NNN = (i.instruction_ & 0xFFF);
    cpu_.pc_ = cpu_.registers_[0] + NNN;
}

void CHIP8::OP_CXKK(Instruction i) {
    std::uint8_t random_byte = RandomByteGenerator::GetInstance().RandomByte();
    uint8_t KK = (i.nibble2_ << 4) | i.nibble1_;

    uint8_t X = i.nibble3_;
    cpu_.registers_[X] = (random_byte & KK);

}

void CHIP8::OP_DXYN(Instruction i) {
    auto vx = i.nibble3_;
    auto vy = i.nibble2_;
    auto n = i.nibble1_;

    auto x = cpu_.registers_[vx] % 64;
    auto y = cpu_.registers_[vy] % 32;

    char sprite_buffer[32];
    std::memcpy(sprite_buffer, cpu_.memory_.data() + cpu_.i_, n);

    for (int i_y = 0; i_y < n; i_y++) {
        auto byte = sprite_buffer[i_y];
        auto y_pos = y + i_y;

        for(int i_x = 0; i_x < 8; i_x++) {
            auto x_pos = x + i_x;

            auto display_pixel = d_->Get(y_pos, x_pos);

            auto mask = 1 << (7 - i_x);
            auto shift = (7 - i_x);
            auto sprite_pixel = (byte & mask) >> shift;

            auto new_pixel_val = sprite_pixel ^ display_pixel;

            if (new_pixel_val == 0 && display_pixel == 1)
                cpu_.registers_[0xF] = 1;
            else
                cpu_.registers_[0xF] = 0;

            d_->Set(y_pos, x_pos, new_pixel_val);

            if (x_pos >= 63)
                break;
        }

        if (y_pos >= 31)
            break;
    }
    draw_flag = true;
}

void CHIP8::OP_EX9E(Instruction i) {
    std::uint8_t X = i.nibble3_;

    if(KeypadKey_State[cpu_.registers_[X]] == KeyState::KeyDown)
        cpu_.pc_ += 2;
}

void CHIP8::OP_EXA1(Instruction i) {
    std::uint8_t X = i.nibble3_;

    std::cout << KeypadKey_State.size() << std::endl;
    for (auto [k, v] : KeypadKey_State)
        std::cout << static_cast<int>(KeypadKey_State[k]) << "  " << static_cast<int>(v) << KeypadKey_State[k] << std::endl;

    if(KeypadKey_State[cpu_.registers_[X]] == KeyState::KeyUp)
        cpu_.pc_ += 2;
}

void CHIP8::OP_FX07(Instruction i) {
    std::uint8_t X = i.nibble3_;

    cpu_.registers_[X] = cpu_.dt_;
}

std::optional<std::uint8_t> KeyDown() {
    for (auto& [ key, key_state ] : KeypadKey_State) {
        if (key_state == KeyState::KeyDown)
            return key;
    }

    return std::nullopt;
}

void CHIP8::OP_FX0A(Instruction i) {
    std::uint8_t X = i.nibble3_;

    auto key = KeyDown();

    if (key == std::nullopt) {
        cpu_.pc_ -= 2;
        return;
    }

    cpu_.registers_[X] = key.value();

}

void CHIP8::OP_FX15(Instruction i) {
    std::uint8_t X = i.nibble3_;

    cpu_.dt_ = cpu_.registers_[X];
}

void CHIP8::OP_FX18(Instruction i) {
    std::uint8_t X = i.nibble3_;

    cpu_.st_ = cpu_.registers_[X];
}

void CHIP8::OP_FX1E(Instruction i) {
    std::uint8_t X = i.nibble3_;

    cpu_.i_ += cpu_.registers_[X];
}

void CHIP8::OP_FX29(Instruction i) {
    std::uint8_t X = i.nibble3_;

    cpu_.i_ = static_cast<int>(X) * FONT_SIZE;
}

void CHIP8::OP_FX33(Instruction i) {
    std::uint8_t Vx = i.nibble3_;
    std::uint8_t num = cpu_.registers_[Vx];

    std::uint8_t ones = num % 10;
    std::uint8_t tens = (num / 10) % 10;
    std::uint8_t hundreds = (num / 100) % 10;

    cpu_.memory_[cpu_.i_ + 2] = static_cast<std::byte>(ones);
    cpu_.memory_[cpu_.i_ + 1]= static_cast<std::byte>(tens);
    cpu_.memory_[cpu_.i_] = static_cast<std::byte>(hundreds);
}

void CHIP8::OP_FX55(Instruction i) {
    std::uint8_t X = i.nibble3_;

    for (int offset = 0x0; offset <= X; offset++) {
        auto register_val = cpu_.registers_[offset];
        cpu_.memory_[cpu_.i_ + offset] = static_cast<std::byte>(cpu_.registers_[offset]);
    }
}

void CHIP8::OP_FX65(Instruction i) {
    std::uint8_t X = i.nibble3_;

    for (int offset = 0x0; offset <= X; offset++) {
        auto memory_val = static_cast<uint8_t>(cpu_.memory_[cpu_.i_ + offset]);
        cpu_.registers_[offset] = static_cast<std::uint8_t>(cpu_.memory_[cpu_.i_ + offset]);
    }
}




#endif //CHIP_8_OPCODES_CPP
