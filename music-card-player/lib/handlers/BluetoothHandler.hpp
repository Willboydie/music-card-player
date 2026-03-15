#pragma once

#include "../event/EventBus.hpp"
#include "../event/Event.hpp"
#include "../bluetooth/IBluetoothManager.hpp"
#include "../storage/DeviceStorage.hpp"
#include <string>
#include <iostream>

class BluetoothHandler {
public:
    BluetoothHandler(EventBus& bus, IBluetoothManager& bluetoothManager, DeviceStorage& storage);

private:
    EventBus& bus;
    IBluetoothManager& bt;
    DeviceStorage& storage;
    
    std::string connectingAddress;
};
