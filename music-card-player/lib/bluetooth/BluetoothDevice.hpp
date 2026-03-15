#pragma once

#include <string>

struct BluetoothDevice {
    std::string address;   // MAC address  e.g. "AA:BB:CC:DD:EE:FF"
    std::string name;      // Human-readable name  e.g. "Sony WH-1000XM5"
};
