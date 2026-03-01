#include "Screen.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cstring>
#include <iostream>


// ── SSD1106 I2C protocol bytes ───────────────────────────────────────────────

static constexpr uint8_t SSD1106_ADDR        = 0x3C;
static constexpr uint8_t CONTROL_COMMAND      = 0x00;   // Co=0, D/C#=0
static constexpr uint8_t CONTROL_DATA         = 0x40;   // Co=0, D/C#=1

// The SSD1106 has 132 columns of internal RAM but the display is only
// 128 pixels wide, centred at a 2-column offset.
static constexpr int COLUMN_OFFSET = 2;


// ── Lifecycle ────────────────────────────────────────────────────────────────

Screen::Screen(int sda_pin, int scl_pin)
    : i2cFd(-1)
    , address(SSD1106_ADDR)
{
    (void)sda_pin;   // pin muxing is handled by the kernel / device tree
    (void)scl_pin;
    pixels.reset();
    std::memset(buffer, 0, sizeof(buffer));
}

Screen::~Screen() {
    if (i2cFd >= 0) {
        sendCommand(0xAE);   // display OFF
        close(i2cFd);
    }
}

bool Screen::init() {
    // ── Open the I2C bus ────────────────────────────────────────
    i2cFd = open("/dev/i2c-1", O_RDWR);
    if (i2cFd < 0) {
        std::cerr << "Screen: failed to open /dev/i2c-1" << std::endl;
        return false;
    }

    if (ioctl(i2cFd, I2C_SLAVE, address) < 0) {
        std::cerr << "Screen: failed to set I2C address 0x"
                  << std::hex << (int)address << std::endl;
        close(i2cFd);
        i2cFd = -1;
        return false;
    }

    // ── SSD1106 initialisation sequence (128×64) ────────────────
    //
    // The SSD1106 only supports page-addressing mode (no horizontal
    // addressing mode like the SSD1306).  Each page must be written
    // individually after setting its address and column pointer.
    static constexpr uint8_t initSequence[] = {
        0xAE,       // Display OFF

        0xD5, 0x80, // Display clock divide / oscillator frequency
        0xA8, 0x3F, // Multiplex ratio → 63 (64 lines)
        0xD3, 0x00, // Display offset → 0
        0x40,       // Start line → 0

        0x8D, 0x14, // Charge pump → enabled
        // (no 0x20 command – SSD1106 only supports page mode)

        0xA1,       // Segment re-map → column 127 → SEG0
        0xC8,       // COM scan direction → remapped

        0xDA, 0x12, // COM pins config → alternative, no L/R remap
        0x81, 0xCF, // Contrast → 207
        0xD9, 0xF1, // Pre-charge period → phase1=1, phase2=15
        0xDB, 0x40, // VCOMH deselect level

        0xA4,       // Entire display ON → follow RAM
        0xA6,       // Normal display (not inverted)
        0xAF        // Display ON
    };

    if (!sendCommands(initSequence, sizeof(initSequence))) {
        std::cerr << "Screen: display init failed" << std::endl;
        close(i2cFd);
        i2cFd = -1;
        return false;
    }

    // Push a blank frame so the display starts clean
    refresh();
    return true;
}


// ── Refresh ──────────────────────────────────────────────────────────────────
//
// The SSD1106 has no horizontal addressing mode, so we must set the page
// and column address before each 128-byte page write.

void Screen::refresh() {
    if (i2cFd < 0) return;

    rasterise();

    for (int page = 0; page < NumPages; ++page) {
        sendPage(page);
    }
}


// ── Page writer ──────────────────────────────────────────────────────────────

bool Screen::sendPage(int page) {
    // Set page address
    if (!sendCommand(0xB0 + page)) return false;

    // Set column address to COLUMN_OFFSET (lower nibble, then upper nibble)
    if (!sendCommand(0x00 + (COLUMN_OFFSET & 0x0F))) return false;
    if (!sendCommand(0x10 + ((COLUMN_OFFSET >> 4) & 0x0F))) return false;

    // Send this page's 128 bytes of pixel data
    return sendData(buffer + page * Width, Width);
}


// ── Rasteriser ───────────────────────────────────────────────────────────────
//
// The bitset is row-major: pixels[y * Width + x].
// The SSD1106 expects page-major data where each byte is a vertical stripe
// of 8 pixels (LSB = topmost row within the page).

void Screen::rasterise() {
    std::memset(buffer, 0, sizeof(buffer));

    for (int page = 0; page < NumPages; ++page) {
        const int rowBase = page * 8;
        for (int x = 0; x < Width; ++x) {
            uint8_t byte = 0;
            for (int bit = 0; bit < 8; ++bit) {
                if (pixels[(rowBase + bit) * Width + x]) {
                    byte |= (1 << bit);
                }
            }
            buffer[page * Width + x] = byte;
        }
    }
}


// ── I2C helpers ──────────────────────────────────────────────────────────────

bool Screen::sendCommand(uint8_t cmd) {
    uint8_t buf[2] = { CONTROL_COMMAND, cmd };
    return write(i2cFd, buf, 2) == 2;
}

bool Screen::sendCommands(const uint8_t* cmds, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i) {
        if (!sendCommand(cmds[i])) return false;
    }
    return true;
}

bool Screen::sendData(const uint8_t* data, std::size_t length) {
    // Each I2C write is prefixed with the data control byte (0x40).
    // We send in chunks to stay within I2C transfer limits.
    static constexpr std::size_t ChunkSize = 128;
    uint8_t packet[1 + ChunkSize];
    packet[0] = CONTROL_DATA;

    std::size_t offset = 0;
    while (offset < length) {
        std::size_t n = std::min(ChunkSize, length - offset);
        std::memcpy(packet + 1, data + offset, n);
        if (write(i2cFd, packet, 1 + n) != static_cast<ssize_t>(1 + n)) {
            return false;
        }
        offset += n;
    }
    return true;
}
