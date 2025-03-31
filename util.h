//
// Created by Dustin on 3/30/25.
//

#ifndef CHIP_8_UTIL_H
#define CHIP_8_UTIL_H

#include <random>

class RandomByteGenerator {
public:
    static RandomByteGenerator& GetInstance() {
        static RandomByteGenerator r;

        return r;
    }

    std::uint8_t RandomByte(){
        return distrib_(gen_);
    }

private:
    RandomByteGenerator() : rd_{}, gen_{rd_()}, distrib_{0, 255} {}

    std::random_device rd_;  // Obtain a random seed from the OS
    std::mt19937 gen_; // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib_; // Define the range
};

#endif //CHIP_8_UTIL_H
