#include "AppController.hpp"

#include "../ui/event/EventBus.hpp"
#include "../ui/button/ButtonListener.hpp"
#include "../ui/screen/Screen.hpp"
#include "../ui/view/Renderer.hpp"
#include "../ui/state_machine/StateMachine.hpp"
#include "../ui/state_machine/StateBuilder.hpp"
#include "../audio/AudioManager.hpp"
#include "../bluetooth/BluetoothManager.hpp"
#include "../handlers/NavigationHandler.hpp"
#include "../handlers/AudioHandler.hpp"
#include "../handlers/BluetoothHandler.hpp"


// GPIO pin assignments for the four push buttons
static constexpr int up_pin     = UP_PIN;
static constexpr int down_pin   = DOWN_PIN;
static constexpr int select_pin = SELECT_PIN;
static constexpr int back_pin   = BACK_PIN;

// I2C pins (informational — actual muxing is in the device tree)
static constexpr int sda_pin = SDA_PIN;
static constexpr int scl_pin = SCL_PIN;


void AppController::run() {

    // ── Core infrastructure ──────────────────────────────────────
    EventBus bus;
    Screen screen(sda_pin, scl_pin);
    screen.init();

    Renderer renderer(screen);

    // ── Managers ─────────────────────────────────────────────────
    AudioManager audio;
    audio.initialise();

    BluetoothManager bluetooth;
    bluetooth.initialise();
    bluetooth.powerOn();

    // ── State machine ────────────────────────────────────────────
    auto stateMachine = buildStateMachine(bus, renderer);

    // ── Input ────────────────────────────────────────────────────
    ButtonListener buttons(bus, up_pin, down_pin, select_pin, back_pin);
    buttons.init();

    // ── Event handlers (subscribe to EventBus, delegate to managers)
    NavigationHandler navigationHandler(bus, *stateMachine);
    AudioHandler      audioHandler(bus, audio);
    BluetoothHandler  bluetoothHandler(bus, bluetooth);

    // ── Main loop ────────────────────────────────────────────────
    while (true) {
        buttons.poll();
        screen.refresh();
    }
}