#include "AppController.hpp"


// GPIO pin assignments for the four push buttons
static constexpr int up_pin     = UP_PIN;
static constexpr int down_pin   = DOWN_PIN;
static constexpr int select_pin = SELECT_PIN;
static constexpr int back_pin   = BACK_PIN;

// I2C pins (informational — actual muxing is in the device tree)
static constexpr int sda_pin = SDA_PIN;
static constexpr int scl_pin = SCL_PIN;


void AppController::run(bool debug) {
    Debugger::debugMode = debug;

    // ── Core infrastructure ──────────────────────────────────────
    EventBus bus;
    DeviceStorage deviceStorage;
    Screen screen(sda_pin, scl_pin);
    screen.init();

    Renderer renderer(screen);

    // ── Managers ─────────────────────────────────────────────────
    AudioManager audio(bus);
    audio.initialise();

    BluetoothManager bluetooth(bus);
    bluetooth.initialise();
    bluetooth.powerOn();

    // ── State machine ────────────────────────────────────────────
    auto stateMachine = buildStateMachine(bus, renderer, screen);

    // ── Input ────────────────────────────────────────────────────
    ButtonListener buttons(bus, up_pin, down_pin, select_pin, back_pin);
    buttons.init();

    // ── Event handlers (subscribe to EventBus, delegate to managers)
    bool running = true;
    bus.subscribe<ExitRequested>([&running](const ExitRequested&) { running = false; });
    NavigationHandler navigationHandler(bus, *stateMachine);
    AudioHandler      audioHandler(bus, audio);
    BluetoothHandler  bluetoothHandler(bus, bluetooth, deviceStorage);

    // ── Main loop ────────────────────────────────────────────────
    while (running) {
        buttons.poll();
        screen.refresh();
    }
}