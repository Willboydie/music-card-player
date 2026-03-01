#include "Button.hpp"

Button::Button(int gpioPin)
    : pin(gpioPin)
    , request(nullptr)
    , wasPressed(false)
    , lastEdgeTime(Clock::time_point::min())
{}

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

bool Button::init(const char *chipname) {
  struct gpiod_line *lineButton;
  int i, val;

  // Open GPIO chip
  chip = gpiod_chip_open_by_name(chipname);

  // Open GPIO lines
  lineButton = gpiod_chip_get_line(chip, pin);

  // Open switch line for input
  gpiod_line_request_input(lineButton, "button");

  return true;
}

bool Button::poll() {
  // Read button status and exit if pressed
  int val = gpiod_line_get_value(lineButton);
  return val == 0;
}

void Button::release() {  // Release lines and chip
  gpiod_line_release(lineButton);
  gpiod_chip_close(chip);
}