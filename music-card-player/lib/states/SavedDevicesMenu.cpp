#include "SavedDevicesMenu.hpp"

void SavedDevicesMenu::loadSavedDevices() {
    auto devices = DeviceStorage::load(DeviceStorage::SAVED_DEVICES_FILE);
    for (const auto& device : devices) {
        std::string address = DeviceStorage::findAddressByName(
            device.name, DeviceStorage::SAVED_DEVICES_FILE);
        items.push_back({ [address](EventBus& b){
            b.publish(OpenConnectingStateRequested{ address }); 
        }, device.name });
        Debugger::debug_msg("SavedDevicesMenu: loaded device " + device.name + " " + address);
    }
}