#pragma once

#include "../event/EventBus.hpp"
#include "../event/Event.hpp"
#include "../bluetooth/IBluetoothManager.hpp"
#include <string>
#include <iostream>

class BluetoothHandler {
public:
    BluetoothHandler(EventBus& bus, IBluetoothManager& bluetoothManager);

private:
    EventBus& bus;
    IBluetoothManager& bt;

    std::string connectingAddress;
};
