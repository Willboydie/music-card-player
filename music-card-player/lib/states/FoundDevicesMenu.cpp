#include "FoundDevicesMenu.hpp"

void FoundDevicesMenu::loadFoundDevices() {
    auto devices = DeviceStorage::load(DeviceStorage::FOUND_DEVICES_FILE);
    for (const auto& device : devices) {
        std::string address = DeviceStorage::findAddressByName(
            device.name, DeviceStorage::FOUND_DEVICES_FILE);
        items.push_back({ [address](EventBus& b){ 
            b.publish(OpenConnectingStateRequested{ address }); 
        }, device.name });
        Debugger::debug_msg("FoundDevicesMenu: loaded device " + device.name + " " + address);
    }
}