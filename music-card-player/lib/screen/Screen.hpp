#pragma once

#include "../utils/constants.h"

#include <bitset>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cstring>
#include <iostream>

// Drives an SSD1106 128×64 I2C OLED display on a Raspberry Pi.
//
// Usage:
//     Screen screen(2, 3);       // SDA / SCL GPIO pins (bus chosen internally)
//     screen.init();             // opens /dev/i2c-1 and configures the display
//     ...                        // write into screen.pixels via the Renderer
//     screen.refresh();          // pushes the full frame to the display

class Screen {
public:
    // ── Constants ────────────────────────────────────────────────
    static constexpr int Width    = SCREEN_WIDTH;   // 128
    static constexpr int Height   = SCREEN_HEIGHT;  // 64
    static constexpr int NumBits  = Width * Height;
    static constexpr int NumBytes = NumBits / 8;
    static constexpr int NumPages = Height / 8;

    // ── Lifecycle ────────────────────────────────────────────────
    explicit Screen(int sda_pin, int scl_pin);
    ~Screen();

    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;

    bool init();
    void refresh();

    // ── Accessors ────────────────────────────────────────────────
    int getWidth()  const { return Width; }
    int getHeight() const { return Height; }

    // ── Frame buffer (row-major: pixel at (x,y) = pixels[y * Width + x])
    std::bitset<SCREEN_WIDTH * SCREEN_HEIGHT> pixels;

private:
    int i2cFd;
    uint8_t address;
    uint8_t buffer[NumBytes];   // SSD1306 page-format buffer

    // ── I2C helpers ──────────────────────────────────────────────
    bool sendCommand(uint8_t cmd);
    bool sendCommands(const uint8_t* cmds, std::size_t count);
    bool sendData(const uint8_t* data, std::size_t length);

    // Convert row-major bitset → SSD1106 page format in buffer
    void rasterise();

    // Write one page (128 bytes) to the display
    bool sendPage(int page);
};
