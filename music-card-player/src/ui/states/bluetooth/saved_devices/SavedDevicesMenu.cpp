#include "SavedDevicesMenu.hpp"
#include "../../../event/Event.hpp"
#include "../../../../storage/DeviceStorage.hpp"
#include "../../../../debugger/Debugger.hpp"

void SavedDevicesMenu::loadSavedDevices() {
    auto devices = DeviceStorage::load(DeviceStorage::MOCK_DEVICES_FILE);
    for (const auto& device : devices) {
        std::string address = DeviceStorage::findAddressByName(
            device.name, DeviceStorage::MOCK_DEVICES_FILE);
        items.push_back({ [address](EventBus& b){
            b.publish(BluetoothConnectionRequested{ address }); 
        }, device.name });
        Debugger::debug_msg("SavedDevicesMenu: loaded device " + device.name + " " + address);
    }
}