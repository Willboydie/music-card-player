#include "ButtonListener.hpp"


ButtonListener::ButtonListener(EventBus& bus,
                               int upPin, int downPin,
                               int selectPin, int backPin)
    : bus(bus)
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
}

bool ButtonListener::init() {

    if (!upButton.init() ||
        !downButton.init() ||
        !selectButton.init() ||
        !backButton.init()) {
        perror("ButtonListener: failed to configure GPIO lines");
        return false;
    }

    return true;
}

void ButtonListener::poll() {
    if (upButton.poll()) {
        bus.publish(UpButtonPressed{});
        Debugger::debug_msg("Up button pressed");
    }
    if (downButton.poll()) {
        bus.publish(DownButtonPressed{});
        Debugger::debug_msg("Down button pressed");
    }
    if (selectButton.poll()) {
        bus.publish(SelectButtonPressed{});
        Debugger::debug_msg("Select button pressed");
    }
    if (backButton.poll()) {
        bus.publish(BackButtonPressed{});
        Debugger::debug_msg("Back button pressed");
    }
}

void ButtonListener::debugPoll() {
    if (upButton.poll()) std::cout << "Up button pressed" << std::endl;
    if (downButton.poll()) std::cout << "Down button pressed" << std::endl;
    if (selectButton.poll()) std::cout << "Select button pressed" << std::endl;
    if (backButton.poll()) std::cout << "Back button pressed" << std::endl;
}
