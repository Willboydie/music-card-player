#include "ButtonListener.hpp"
#include <iostream>

static constexpr const char* GPIO_CHIP = "/dev/gpiochip4"; // /dev/gpiochip0 for Pi Zero 2 W


ButtonListener::ButtonListener(EventBus& bus,
                               int upPin, int downPin,
                               int selectPin, int backPin)
    : bus(bus)
    , chip(nullptr)
    , upButton(upPin)
    , downButton(downPin)
    , selectButton(selectPin)
    , backButton(backPin)
{}

ButtonListener::~ButtonListener() {
    upButton.release();
    downButton.release();
    selectButton.release();
    backButton.release();

    if (chip) {
        gpiod_chip_close(chip);
        chip = nullptr;
    }
}

bool ButtonListener::init() {
    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        std::cerr << "ButtonListener: failed to open " << GPIO_CHIP << std::endl;
        return false;
    }

    if (!upButton.init(chip) ||
        !downButton.init(chip) ||
        !selectButton.init(chip) ||
        !backButton.init(chip)) {
        std::cerr << "ButtonListener: failed to configure GPIO lines" << std::endl;
        return false;
    }

    return true;
}

void ButtonListener::poll() {
    if (upButton.poll())     bus.publish(UpButtonPressed{});
    if (downButton.poll())   bus.publish(DownButtonPressed{});
    if (selectButton.poll()) bus.publish(SelectButtonPressed{});
    if (backButton.poll())   bus.publish(BackButtonPressed{});
}

void ButtonListener::debugPoll() {
    if (upButton.poll()) std::cout << "Up button pressed" << std::endl;
    if (downButton.poll()) std::cout << "Down button pressed" << std::endl;
    if (selectButton.poll()) std::cout << "Select button pressed" << std::endl;
    if (backButton.poll()) std::cout << "Back button pressed" << std::endl;
}
