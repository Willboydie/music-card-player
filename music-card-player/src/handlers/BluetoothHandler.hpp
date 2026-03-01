#pragma once

#include "../ui/event/EventBus.hpp"
#include "../ui/event/Event.hpp"
#include "../bluetooth/IBluetoothManager.hpp"
#include <string>

class BluetoothHandler {
public:
    BluetoothHandler(EventBus& bus, IBluetoothManager& bluetoothManager);

private:
    EventBus& bus;
    IBluetoothManager& bt;

    std::string connectingAddress;
};
