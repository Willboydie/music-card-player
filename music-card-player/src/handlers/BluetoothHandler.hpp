#pragma once

#include "../ui/event/EventBus.hpp"
#include "../ui/event/Event.hpp"
#include "../bluetooth/BluetoothManager.hpp"
#include <string>

// Subscribes to Bluetooth-related events on the EventBus and
// dispatches them to the BluetoothManager.
//
// On search completion the handler persists the found devices and
// publishes BluetoothDeviceSearchComplete so the UI can navigate.
//
// On connection success/failure it publishes BluetoothConnected or
// BluetoothConnectionFailed for the NavigationHandler to pick up.

class BluetoothHandler {
public:
    BluetoothHandler(EventBus& bus, BluetoothManager& bluetoothManager);

private:
    EventBus& bus;
    BluetoothManager& bt;

    // Tracks the MAC address of the device currently being connected
    std::string connectingAddress;
};
