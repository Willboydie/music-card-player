#pragma once

#include "BluetoothDevice.hpp"
#include <gio/gio.h>
#include <string>
#include <vector>

// Manages the BlueZ D-Bus interface on a Raspberry Pi.
//
// Discovery results are persisted to FOUND_DEVICES_FILE automatically.
// Successfully paired devices are persisted to SAVED_DEVICES_FILE.
//
// Typical usage:
//     BluetoothManager bt;
//     bt.initialise();
//     bt.powerOn();
//     bt.startDiscovery();
//     ...
//     auto devices = bt.completeDiscovery();   // stops scan + saves found list
//     bt.pairAndSave(device);                  // pair + trust + persist
//     bt.connect(device.address);

class BluetoothManager {
public:
    BluetoothManager();
    ~BluetoothManager();

    // Non-copyable
    BluetoothManager(const BluetoothManager&) = delete;
    BluetoothManager& operator=(const BluetoothManager&) = delete;

    // ── Lifecycle ────────────────────────────────────────────────
    bool initialise();
    void shutdown();

    // ── Adapter ──────────────────────────────────────────────────
    bool powerOn();

    // ── Discovery ────────────────────────────────────────────────
    bool startDiscovery();
    bool stopDiscovery();

    // Queries BlueZ for all discovered devices.
    std::vector<BluetoothDevice> getDiscoveredDevices();

    // Stops discovery, queries devices, and persists the list to
    // found_devices.txt.  Returns the device list.
    std::vector<BluetoothDevice> completeDiscovery();

    // ── Device management ────────────────────────────────────────
    bool pair(const std::string& mac);
    bool trust(const std::string& mac);
    bool connect(const std::string& mac);
    bool disconnect(const std::string& mac);

    // Pairs + trusts a device and persists it to saved_devices.txt.
    bool pairAndSave(const BluetoothDevice& device);

    // ── Stored device access ─────────────────────────────────────
    std::vector<BluetoothDevice> getSavedDevices() const;
    std::vector<BluetoothDevice> getFoundDevices() const;

    // ── Status ───────────────────────────────────────────────────
    bool isConnected(const std::string& mac);

private:
    GDBusConnection* dbus;
    std::string adapterPath;

    // ── D-Bus helpers ────────────────────────────────────────────
    std::string devicePath(const std::string& mac) const;

    bool callMethod(const std::string& objectPath,
                    const std::string& interface,
                    const std::string& method,
                    GVariant* params = nullptr,
                    int timeoutMs = -1);

    bool setProperty(const std::string& objectPath,
                     const std::string& interface,
                     const std::string& property,
                     GVariant* value);

    GVariant* getProperty(const std::string& objectPath,
                          const std::string& interface,
                          const std::string& property);
};
