#pragma once

#include "Button.hpp"
#include "../event/EventBus.hpp"
#include "../event/Event.hpp"
#include <gpiod.h>

// Polls four GPIO push-buttons each loop iteration and publishes
// the corresponding Event to the EventBus on every debounced press.
//
// Usage:
//     EventBus bus;
//     ButtonListener buttons(bus, UP_PIN, DOWN_PIN, SELECT_PIN, BACK_PIN);
//     buttons.init();
//     while (true) { buttons.poll(); ... }

class ButtonListener {
public:
    ButtonListener(EventBus& bus,
                   int upPin, int downPin,
                   int selectPin, int backPin);
    ~ButtonListener();

    ButtonListener(const ButtonListener&) = delete;
    ButtonListener& operator=(const ButtonListener&) = delete;

    bool init();
    void poll();
    void debugPoll();

private:
    EventBus& bus;
    struct gpiod_chip* chip;

    Button upButton;
    Button downButton;
    Button selectButton;
    Button backButton;
};
