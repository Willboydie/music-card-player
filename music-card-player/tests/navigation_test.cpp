#include "../src/utils/constants.h"

#include "../src/ui/event/EventBus.hpp"
#include "../src/ui/button/ButtonListener.hpp"
#include "../src/ui/screen/Screen.hpp"
#include "../src/ui/view/Renderer.hpp"
#include "../src/ui/state_machine/StateMachine.hpp"
#include "../src/ui/state_machine/StateBuilder.hpp"
#include "../src/audio/MockAudioManager.hpp"
#include "../src/bluetooth/MockBluetoothManager.hpp"
#include "../src/handlers/NavigationHandler.hpp"
#include "../src/handlers/AudioHandler.hpp"
#include "../src/handlers/BluetoothHandler.hpp"
#include "../src/debugger/Debugger.hpp"

#include <iostream>
#include <chrono>
#include <thread>


// GPIO pin assignments for the four push buttons
static constexpr int up_pin     = UP_PIN;
static constexpr int down_pin   = DOWN_PIN;
static constexpr int select_pin = SELECT_PIN;
static constexpr int back_pin   = BACK_PIN;

// I2C pins (informational — actual muxing is in the device tree)
static constexpr int sda_pin = SDA_PIN;
static constexpr int scl_pin = SCL_PIN;


static void navigation_test() {
    // auto start_time = std::chrono::steady_clock::now();

    Debugger::debugMode = true;

    // ── Core infrastructure ──────────────────────────────────────
    EventBus bus;
    Screen screen(sda_pin, scl_pin);
    screen.init();

    Renderer renderer(screen);

    // ── Managers ─────────────────────────────────────────────────
    MockAudioManager audio(bus);
    audio.initialise();

    MockBluetoothManager bluetooth(bus);
    bluetooth.initialise();
    bluetooth.powerOn();

    // ── State machine ────────────────────────────────────────────
    auto stateMachine = buildStateMachine(bus, renderer);

    // ── Input ────────────────────────────────────────────────────
    ButtonListener buttons(bus, up_pin, down_pin, select_pin, back_pin);
    buttons.init();

    // ── Event handlers ─────────────────────────────────────────
    NavigationHandler navigationHandler(bus, *stateMachine);
    AudioHandler      audioHandler(bus, audio);
    BluetoothHandler  bluetoothHandler(bus, bluetooth);

    // ── Main loop ────────────────────────────────────────────────
    while (true) {
        buttons.poll();
        screen.refresh();
        
        // if (std::chrono::steady_clock::now() - start_time > std::chrono::seconds(1)) {
        //     if (std::cin.get() == 'q') {
        //         std::cout << "Navigation test: Exiting..." << std::endl;
        //         break;
        //     }
        // }
    }
}

int main() {
    navigation_test();
    return 0;
}