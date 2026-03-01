#include "BluetoothHandler.hpp"
#include <iostream>

BluetoothHandler::BluetoothHandler(EventBus& bus, IBluetoothManager& bluetoothManager)
    : bus(bus)
    , bt(bluetoothManager)
{
    // ── Discovery ────────────────────────────────────────────────
    bus.subscribe<BluetoothDeviceSearchRequested>([&](const BluetoothDeviceSearchRequested&) {
        if (!bt.startDiscovery()) {
            std::cerr << "BluetoothHandler: failed to start discovery" << std::endl;
        }
    });

    bus.subscribe<BluetoothDeviceSearchAbortRequested>([&](const BluetoothDeviceSearchAbortRequested&) {
        // Stop discovery, persist whatever was found, and notify the UI
        bt.completeDiscovery();
        bus.publish(BluetoothDeviceSearchComplete{});
    });

    // ── Connection ───────────────────────────────────────────────
    bus.subscribe<BluetoothConnectionRequested>([&](const BluetoothConnectionRequested& e) {
        connectingAddress = e.device_info;   // MAC address

        // Look up the device name from the found-devices list
        BluetoothDevice device{ connectingAddress, connectingAddress };
        for (const auto& d : bt.getFoundDevices()) {
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
