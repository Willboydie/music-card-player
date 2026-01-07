#pragma once

#include "constants.h"
#include <bitset>
#include <fstream>
#include <stdexcept>
#include <string>

class Screen {
public:
    static constexpr std::size_t NumBits  = SCREEN_WIDTH * SCREEN_HEIGHT;
    static constexpr std::size_t NumBytes = (NumBits + 7) / 8;

    explicit Screen(const std::string& filename) {
        load(filename);
    }

    std::bitset<NumBits> pixels;

private:
    void load(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Could not open file: " + filename);
        }

        unsigned char buffer[NumBytes]{};
        file.read(reinterpret_cast<char*>(buffer), NumBytes);

        if (!file) {
            throw std::runtime_error("File too small: " + filename);
        }

        // Unpack bits (LSB-first)
        for (std::size_t i = 0; i < NumBits; ++i) {
            pixels[i] = (buffer[i / 8] >> (i % 8)) & 1;
        }
    }
};
