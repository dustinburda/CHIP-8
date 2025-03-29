//
// Created by Dustin on 3/6/25.
//

#ifndef CHIP_8_OPCODES_H
#define CHIP_8_OPCODES_H

void CPU::OP_00E0() {
    std::memset(d_->GetBuffer().data(), sizeof(uint8_t) * WIDTH * HEIGHT, 0x0);
}

void CPU::OP_00EE() {
    state_.pc_ = state_.stack_.top();
    state_.stack_.pop();
}

void CPU::OP_1NNN(Instruction i) {
    std::uint16_t nnn = 0x0;

    nnn |= (i.nibble1_ | (i.nibble2_ << 4) | (i.nibble3_ << 8));
    state_.pc_ = nnn;
}

void CPU::OP_2NNN(Instruction i) {
    state_.stack_.push(state_.pc_);

    std::uint16_t nnn = 0x0;
    nnn |= (i.nibble1_ | (i.nibble2_ << 4) | (i.nibble3_ << 8));
    state_.pc_ = nnn;

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

}

void CPU::OP_BNNN(Instruction i) {

}

void CPU::OP_CXKK(Instruction i) {

}

void CPU::OP_DXYN(Instruction i) {

}

void CPU::OP_EX9E(Instruction i) {

}

void CPU::OP_EXA1(Instruction i) {

}

void CPU::OP_FX07(Instruction i) {

}

void CPU::OP_FX0A(Instruction i) {

}

void CPU::OP_FX15(Instruction i) {

}

void CPU::OP_FX18(Instruction i) {

}

void CPU::OP_FX1E(Instruction i) {

}

void CPU::OP_FX29(Instruction i) {

}

void CPU::OP_FX33(Instruction i) {

}

void CPU::OP_FX55(Instruction i) {

}

void CPU::OP_FX65(Instruction i) {

}




#endif //CHIP_8_OPCODES_H
