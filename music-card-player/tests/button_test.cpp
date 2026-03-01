#include "../src/ui/button/Button.hpp"
#include "../src/ui/button/ButtonListener.hpp"
#include <iostream>
#include "../src/utils/constants.h"
#include <thread>
#include <chrono>

static void button_test() {
    EventBus bus;
    ButtonListener buttonListener(bus, UP_PIN, DOWN_PIN, SELECT_PIN, BACK_PIN);

    std::cout << "Beginning button test\n";
    if (!buttonListener.init()) {
        std::cerr << "ButtonListener init failed. Exiting...\n";
        std::exit(1);
    }
    std::cout << "ButtonListener initialized. Beginning test loop\n";

    while (true) {
        buttonListener.debugPoll();
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main() {
    button_test();
    return 0;
}