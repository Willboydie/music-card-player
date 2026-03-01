#include "../src/ui/button/Button.hpp"
#include "../src/ui/button/ButtonListener.hpp"
#include <iostream>
#include "../src/utils/constants.h"
#include <thread>
#include <chrono>

static void button_test() {
    Button upButton(UP_PIN);
    Button downButton(DOWN_PIN);
    Button selectButton(SELECT_PIN);
    Button backButton(BACK_PIN);
    if (!upButton.init() || !downButton.init() || !selectButton.init() || !backButton.init()) {
        perror("Button: init failed");
        return;
    }
    std::cout << "Buttons initialized. Polling..." << std::endl;
    int up_count = 0;
    int down_count = 0;
    int select_count = 0;
    int back_count = 0;
    while (up_count < 10 && down_count < 10 && select_count < 10 && back_count < 10) {
        if (upButton.poll()) {
            up_count ++;
            std::cout << "Up button pressed " << up_count << " times" << std::endl; 
        }
        if (downButton.poll()) {
            down_count ++;
            std::cout << "Down button pressed " << down_count << " times" << std::endl;
        }
        if (selectButton.poll()) {
            select_count ++;
            std::cout << "Select button pressed " << select_count << " times" << std::endl;
        }
        if (backButton.poll()) {
            back_count ++;
            std::cout << "Back button pressed " << back_count << " times" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    upButton.release();
    downButton.release();
    selectButton.release();
    backButton.release();
    std::cout << "Buttons released. Exiting..." << std::endl;
}

int main() {
    button_test();
    return 0;
}