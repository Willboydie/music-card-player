#include "MockBluetoothManager.hpp"

bool MockBluetoothManager::initialise() { 
    Debugger::debug_msg("MockBluetoothManager: initialised");
    return true; 
}
void MockBluetoothManager::shutdown() {
    Debugger::debug_msg("MockBluetoothManager: shutting down");
}

bool MockBluetoothManager::powerOn() {
    Debugger::debug_msg("MockBluetoothManager: powered on");
    return true;
}

void MockBluetoothManager::discoverDevices() {
    Debugger::debug_msg("MockBluetoothManager: searching for devices...");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    Debugger::debug_msg("Search complete.");
    eventBus_.publish(BluetoothDeviceSearchComplete{});
    return;
}

void MockBluetoothManager::abortDiscovery() {
    Debugger::debug_msg("MockBluetoothManager: aborting discovery");
    return;
}

bool MockBluetoothManager::connect(const std::string& mac) {
    Debugger::debug_msg("MockBluetoothManager: connecting... " + mac);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    connected_ = true;
    connected_mac = mac;
    eventBus_.publish(BluetoothConnected{});
    return true;
}
bool MockBluetoothManager::disconnect(const std::string& mac) {
    Debugger::debug_msg("MockBluetoothManager: disconnected device " + mac);
    connected_ = false;
    return true;
}

bool MockBluetoothManager::pairAndSave(const BluetoothDevice& device) {
    Debugger::debug_msg("MockBluetoothManager: pairing and saving " + device.address);
    return true;
}

bool MockBluetoothManager::pair(const std::string& mac) {
    Debugger::debug_msg("MockBluetoothManager: pairing " + mac);
    return true;
}
bool MockBluetoothManager::trust(const std::string& mac) {
    Debugger::debug_msg("MockBluetoothManager: trusting " + mac);
    return true;
}