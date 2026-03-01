#pragma once

#include <gpiod.h>
#include <chrono>

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
    bool init(struct gpiod_chip* chip);

    // Read the pin.  Returns true once per press (falling-edge, debounced).
    bool poll();

    void release();

private:
    int pin;
    struct gpiod_chip *chip;

    // using Clock = std::chrono::steady_clock;
    // static constexpr std::chrono::milliseconds DebouncePeriod{50};
    // gpiod_line_request* request = nullptr;
    // unsigned int offset_ = 0;
    // bool wasPressed = false;
    // bool lastStableState = false;
    // Clock::time_point lastEdgeTime;
};
