#include "SavedDevicesMenu.hpp"
#include "../../../event/Event.hpp"
#include "../../../../storage/DeviceStorage.hpp"

void SavedDevicesMenu::loadSavedDevices() {
    auto devices = DeviceStorage::load(DeviceStorage::SAVED_DEVICES_FILE);
    int index = 0;
    for (const auto& device : devices) {
        auto option = static_cast<MenuOption>(
            static_cast<int>(MenuOption::DynamicStartDevices) + index);
        std::string address = DeviceStorage::findAddressByName(
            device.name, DeviceStorage::SAVED_DEVICES_FILE);
        items.push_back({ option, [address](EventBus& b){ b.publish(BluetoothConnectionRequested{ address }); }, device.name });
        ++index;
    }
}