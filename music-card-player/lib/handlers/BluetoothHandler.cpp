#include "BluetoothHandler.hpp"

BluetoothHandler::BluetoothHandler(EventBus& bus, IBluetoothManager& bluetoothManager, DeviceStorage& storage)
    : bus(bus)
    , bt(bluetoothManager)
    , storage(storage)
{
    // ── Discovery ────────────────────────────────────────────────
    bus.subscribe<BluetoothDeviceSearchRequested>([&](const BluetoothDeviceSearchRequested&) {
        bt.discoverDevices();
    });

    bus.subscribe<BluetoothDeviceSearchAbortRequested>([&](const BluetoothDeviceSearchAbortRequested&) {
        // Stop discovery, persist whatever was found, and notify the UI
        bt.abortDiscovery();
        bus.publish(BluetoothDeviceSearchComplete{});
    });

    // ── Connection ───────────────────────────────────────────────
    bus.subscribe<BluetoothConnectionRequested>([&](const BluetoothConnectionRequested& e) {
        connectingAddress = e.device_info;   // MAC address

        // Look up the device name from the found-devices list
        BluetoothDevice device{ connectingAddress, connectingAddress };
        for (const auto& d : storage.load(DeviceStorage::FOUND_DEVICES_FILE)) {
            if (d.address == connectingAddress) {
                device.name = d.name;
                break;
            }
        }

        // Pair, trust, persist, then connect
        if (bt.pairAndSave(device) && bt.connect(connectingAddress)) {
            bus.publish(BluetoothConnected{});
        } else {
            std::cerr << "BluetoothHandler: connection failed for "
                      << connectingAddress << std::endl;
            bus.publish(BluetoothConnectionFailed{});
        }
    });

    bus.subscribe<BluetoothConnectionAbortRequested>([&](const BluetoothConnectionAbortRequested&) {
        if (!connectingAddress.empty()) {
            bt.disconnect(connectingAddress);
            connectingAddress.clear();
        }
    });
}