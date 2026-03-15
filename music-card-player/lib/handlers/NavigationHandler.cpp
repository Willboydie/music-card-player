#include "NavigationHandler.hpp"

NavigationHandler::NavigationHandler(EventBus& bus, StateMachine& stateMachine)
    : bus(bus)
    , sm(stateMachine)
{
    // ── Forward button presses to the current state ──────────────
    bus.subscribe<UpButtonPressed>([&](const UpButtonPressed& e) {
        sm.onEvent(e);
    });

    bus.subscribe<DownButtonPressed>([&](const DownButtonPressed& e) {
        sm.onEvent(e);
    });

    bus.subscribe<SelectButtonPressed>([&](const SelectButtonPressed& e) {
        sm.onEvent(e);
    });

    bus.subscribe<BackButtonPressed>([&](const BackButtonPressed& e) {
        sm.onEvent(e);
    });

    // ── Navigation requests → state transitions ──────────────────
    bus.subscribe<OpenMainMenuStateRequested>([&](const OpenMainMenuStateRequested&) {
        sm.transitionTo(StateId::MAIN_MENU);
    });

    bus.subscribe<OpenPlayerStateRequested>([&](const OpenPlayerStateRequested&) {
        sm.transitionTo(StateId::PLAYER);
    });

    bus.subscribe<OpenBluetoothMenuStateRequested>([&](const OpenBluetoothMenuStateRequested&) {
        sm.transitionTo(StateId::BLUETOOTH_MENU);
    });

    bus.subscribe<OpenSavedDevicesMenuRequested>([&](const OpenSavedDevicesMenuRequested&) {
        sm.transitionTo(StateId::SAVED_DEVICES);
    });

    bus.subscribe<OpenConnectNewMenuRequested>([&](const OpenConnectNewMenuRequested&) {
        sm.transitionTo(StateId::CONNECT_NEW);
    });

    bus.subscribe<OpenFoundDevicesMenuRequested>([&](const OpenFoundDevicesMenuRequested&) {
        sm.transitionTo(StateId::FOUND_DEVICES);
    });
    bus.subscribe<OpenSearchingForDevicesMenuRequested>([&](const OpenSearchingForDevicesMenuRequested&) {
        sm.transitionTo(StateId::SEARCHING_FOR_DEVICES);
    });
    bus.subscribe<OpenConnectingStateRequested>([&](const OpenConnectingStateRequested& e) {
        sm.getState(StateId::CONNECTING)->info = e.device_info;
        sm.transitionTo(StateId::CONNECTING);
    });


    // ── Outcome events that also drive navigation ────────────────

    bus.subscribe<BluetoothDeviceSearchRequested>([&](const BluetoothDeviceSearchRequested&) {
        sm.transitionTo(StateId::SEARCHING_FOR_DEVICES);
    });

    bus.subscribe<BluetoothDeviceSearchAbortRequested>([&](const BluetoothDeviceSearchAbortRequested&) {
        sm.transitionTo(StateId::BLUETOOTH_MENU);
    });

    bus.subscribe<BluetoothDeviceSearchComplete>([&](const BluetoothDeviceSearchComplete&) {
        sm.transitionTo(StateId::FOUND_DEVICES);
    });

    bus.subscribe<BluetoothConnected>([&](const BluetoothConnected&) {
        sm.transitionTo(StateId::MAIN_MENU);
    });

    bus.subscribe<BluetoothConnectionFailed>([&](const BluetoothConnectionFailed&) {
        sm.transitionTo(StateId::BLUETOOTH_MENU);
    });

    bus.subscribe<BluetoothConnectionAbortRequested>([&](const BluetoothConnectionAbortRequested&) {
        sm.transitionTo(StateId::BLUETOOTH_MENU);
    });
}
