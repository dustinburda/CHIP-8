//
// Created by Dustin on 3/6/25.
//

#ifndef CHIP_8_OPCODES_CPP
#define CHIP_8_OPCODES_CPP

#include "../include/util.h"

#include "../include/CPU.h"
#include "../include/Font.h"

void CPU::OP_00E0() {
    d_->ClearDisplay();
}

void CPU::OP_00EE() {
    state_.pc_ = state_.stack_.top();
    state_.stack_.pop();
}

void CPU::OP_1NNN(Instruction i) {
    std::uint16_t NNN = (i.instruction_ & 0xFFF);
    state_.pc_ = NNN;
}

void CPU::OP_2NNN(Instruction i) {
    state_.stack_.push(state_.pc_);

    std::uint16_t NNN = (i.instruction_ & 0xFFF);
    state_.pc_ = NNN;

}

void CPU::OP_3XKK(Instruction i) {
    uint8_t X = i.nibble3_;

    uint8_t KK = (i.nibble2_ << 4) | i.nibble1_;

    if (state_.registers_[X] == KK)
        state_.pc_ += 2;
}

void CPU::OP_4XKK(Instruction i) {
    uint8_t X = i.nibble3_;

    uint8_t KK = (i.nibble2_ << 4) | i.nibble1_;

    if (state_.registers_[X] != KK)
        state_.pc_ += 2;
}

void CPU::OP_5XY0(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    if(state_.registers_[X] == state_.registers_[Y])
        state_.pc_ += 2;
}

void CPU::OP_6XKK(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t KK = (i.nibble2_ << 4) | i.nibble1_;

    state_.registers_[X] = KK;
}

void CPU::OP_7XKK(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t KK = (i.nibble2_ << 4) | i.nibble1_;

    state_.registers_[X] += KK;
}

void CPU::OP_8XY1(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    state_.registers_[X] = state_.registers_[Y];
}

void CPU::OP_8XY2(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    state_.registers_[X] = (state_.registers_[X] | state_.registers_[Y]);
}

void CPU::OP_8XY3(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    state_.registers_[X] = (state_.registers_[X] | state_.registers_[Y]);
}

void CPU::OP_8XY4(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    state_.registers_[0xF] = (X > 255 - Y);
    state_.registers_[X] += state_.registers_[Y];
}

void CPU::OP_8XY5(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    // TODO: GEQ or EQ?
    state_.registers_[0xF] = (X > Y);
    state_.registers_[X] -= state_.registers_[Y];
}

void CPU::OP_8XY6(Instruction i) {
    uint8_t X = i.nibble3_;

    uint8_t LSB = X & 0x1;
    state_.registers_[X] >>= 1;
    state_.registers_[0xF] = LSB;
}

void CPU::OP_8XY7(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    // TODO: GEQ or EQ?
    state_.registers_[0xF] = (Y > X);
    state_.registers_[Y] -= state_.registers_[X];
}

void CPU::OP_8XYE(Instruction i) {
    uint8_t X = i.nibble3_;

    uint8_t LSB = (X & (0x1 << 7)) >> 7;
    state_.registers_[X] <<= 1;
    state_.registers_[0xF] = LSB;
}

void CPU::OP_9XY0(Instruction i) {
    uint8_t X = i.nibble3_;
    uint8_t Y = i.nibble2_;

    if(state_.registers_[X] != state_.registers_[Y])
        state_.pc_ += 2;
}

void CPU::OP_ANNN(Instruction i) {
    std::uint16_t NNN = (i.instruction_ & 0xFFF);
    state_.i_ = NNN;
}

void CPU::OP_BNNN(Instruction i) {
    std::uint16_t NNN = (i.instruction_ & 0xFFF);
    state_.pc_ = state_.registers_[0] + NNN;
}

void CPU::OP_CXKK(Instruction i) {
    std::uint8_t random_byte = RandomByteGenerator::GetInstance().RandomByte();
    uint8_t KK = (i.nibble2_ << 4) | i.nibble1_;

    uint8_t X = i.nibble3_;
    state_.registers_[X] = (random_byte & KK);

}

void CPU::OP_DXYN(Instruction i) {
    auto vx = i.nibble3_;
    auto vy = i.nibble2_;
    auto n = i.nibble1_;

    auto x = state_.registers_[vx] % 64;
    auto y = state_.registers_[vy] % 32;

    char sprite_buffer[32];
    std::memcpy(sprite_buffer, state_.memory_.data() + state_.i_, n);

    for (int i_y = 0; i_y < n; i_y++) {
        auto byte = sprite_buffer[i_y];
        for(int i_x = 0; i_x < 8; i_x++) {
            auto x_pos = x + i_x;
            auto y_pos = y + i_y;

            if (x_pos >= 64)
                break;

            auto old_pixel_val = d_->Get(y_pos, x_pos);
            auto new_pixel_val = ((byte && (1 << (7 - i_x))) >> (7 - i_x)) ^ old_pixel_val;

            if (old_pixel_val == 1 && new_pixel_val == 0)
                state_.registers_[0xF] = 1;

            d_->Set(y_pos, x_pos, new_pixel_val);
        }

        if (y >= 32)
            break;
    }
}

void CPU::OP_EX9E(Instruction i) {
    std::uint8_t X = i.nibble3_;

    if(KeypadKey_State[X] == KeyState::KeyDown)
        state_.pc_ += 2;
}

void CPU::OP_EXA1(Instruction i) {
    std::uint8_t X = i.nibble3_;

    if(KeypadKey_State[X] == KeyState::KeyUp)
        state_.pc_ += 2;
}

void CPU::OP_FX07(Instruction i) {
    std::uint8_t X = i.nibble3_;

    state_.registers_[X] = state_.dt_;
}

std::optional<std::uint8_t> KeyDown() {
    for (auto& [ key, key_state ] : KeypadKey_State) {
        if (key_state == KeyState::KeyDown)
            return key;
    }

    return std::nullopt;
}

void CPU::OP_FX0A(Instruction i) {
    std::uint8_t X = i.nibble3_;

    auto key = KeyDown();

    if (key == std::nullopt) {
        state_.pc_ -= 2;
        return;
    }

    state_.registers_[X] = key.value();

}

void CPU::OP_FX15(Instruction i) {
    std::uint8_t X = i.nibble3_;

    state_.dt_ = state_.registers_[X];
}

void CPU::OP_FX18(Instruction i) {
    std::uint8_t X = i.nibble3_;

    state_.st_ = state_.registers_[X];
}

void CPU::OP_FX1E(Instruction i) {
    std::uint8_t X = i.nibble3_;

    state_.i_ += state_.registers_[X];
}

void CPU::OP_FX29(Instruction i) {
    std::uint8_t X = i.nibble3_;

    state_.i_ = static_cast<int>(X) * FONT_SIZE;
}

void CPU::OP_FX33(Instruction i) {
    std::uint8_t X = i.nibble3_;

    std::uint8_t ones = X % 10;
    std::uint8_t tens = X % 100;
    std::uint8_t hundreds = X % 1000;

    state_.memory_[state_.i_] = static_cast<std::byte>(ones);
    state_.memory_[state_.i_]= static_cast<std::byte>(tens);
    state_.memory_[state_.i_] = static_cast<std::byte>(hundreds);
}

void CPU::OP_FX55(Instruction i) {
    std::uint8_t X = i.nibble3_;

    for (int offset = 0x0; offset < X; offset++) {
        state_.memory_[state_.i_ + offset] = static_cast<std::byte>(state_.registers_[offset]);
    }
}

void CPU::OP_FX65(Instruction i) {
    std::uint8_t X = i.nibble3_;

    for (int offset = 0x0; offset < X; offset++) {
        state_.registers_[offset] = static_cast<std::uint8_t>(state_.memory_[state_.i_ + offset]);
    }
}




#endif //CHIP_8_OPCODES_CPP
