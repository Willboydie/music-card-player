#include "FoundDevicesMenu.hpp"
#include "../../../../event/Event.hpp"

void FoundDevicesMenu::loadFoundDevices() {
    auto devices = DeviceStorage::load(DeviceStorage::FOUND_DEVICES_FILE);
    int index = 0;
    for (const auto& device : devices) {
        auto option = static_cast<MenuOption>(
            static_cast<int>(MenuOption::DynamicStartDevices) + index);
        std::string address = DeviceStorage::findAddressByName(
            device.name, DeviceStorage::FOUND_DEVICES_FILE);
        items.push_back({ option, [address](EventBus& b){ b.publish(BluetoothConnectionRequested{ address }); }, device.name });
        ++index;
    }
}