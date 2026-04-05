#pragma once

#include <gpiod.h>
#include <iostream>
#include <poll.h>

static constexpr const char* GPIO_CHIP_PATH = "../../../dev/gpiochip4"; // raspi5: 0, zero2W: 4

// Represents a single active-low push button on a GPIO pin.
//
// The button connects the GPIO to GND when pressed, so the line is
// configured with an internal pull-up and reads LOW when pressed.
//
// poll() returns true exactly once per debounced press (falling edge).

class Button {
public:
    explicit Button(int gpioPin);

    // Configure the GPIO line on the given chip.  Must be called before poll().
    bool init();

    // Read the pin.  Returns true once per press (falling-edge, debounced).
    bool poll();
    void release();

private:
    unsigned int pin;
    const char *chip_path = GPIO_CHIP_PATH;
    gpiod_chip *chip = nullptr;

    gpiod_line_request* request = nullptr;
    gpiod_edge_event_buffer* buffer = nullptr;

    int fd = -1;
};
