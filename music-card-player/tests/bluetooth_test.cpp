#include "../lib/bluetooth/BluetoothManager.hpp"
#include "../lib/debugger/Debugger.hpp"
#include "../lib/event/EventBus.hpp"
#include "../lib/storage/DeviceStorage.hpp"

static void bluetooth_test() {
    Debugger::debugMode = true;
    EventBus bus;
    DeviceStorage deviceStorage;
    BluetoothManager bluetooth(bus);

    bluetooth.initialise();
    bluetooth.powerOn();
    bluetooth.discoverDevices();
    auto devices = deviceStorage.load(DeviceStorage::FOUND_DEVICES_FILE);
    for (const auto& device : devices) {
        std::cout << device.name << " - " << device.address << std::endl;
    }
    bluetooth.shutdown();
}

int main() {
    bluetooth_test();
    return 0;
}