#pragma once

#include "../bluetooth/BluetoothDevice.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "../debugger/Debugger.hpp"

// Flat-file persistence for BluetoothDevice lists.
//
// Paths are relative to the process current working directory unless
// MUSIC_CARD_PLAYER_HOME is set (absolute base directory for data files).
//
// Two predefined files:
//   SAVED_DEVICES_FILE  – devices the user has paired with (persistent)
//   FOUND_DEVICES_FILE  – devices discovered during the most recent scan (transient)
//
// File format (one device per line):
//   name|AA:BB:CC:DD:EE:FF
//
// Lines starting with '#' are comments and are skipped on load.

class DeviceStorage {
public:
    static constexpr const char* SAVED_DEVICES_FILE = "data/devices/saved_devices.txt";
    static constexpr const char* FOUND_DEVICES_FILE = "data/devices/found_devices.txt";
    static constexpr const char* MOCK_DEVICES_FILE = "data/devices/mock_devices.txt";

    // ── Bulk I/O ─────────────────────────────────────────────────
    static std::vector<BluetoothDevice> load(const std::string& filepath);
    static bool save(const std::vector<BluetoothDevice>& devices,
                     const std::string& filepath);

    // ── Lookup ────────────────────────────────────────────────────
    // Returns the MAC address for the first device whose name matches,
    // or an empty string if not found.
    static std::string findAddressByName(const std::string& name,
                                         const std::string& filepath);

    // ── Convenience helpers (load → modify → save) ──────────────
    // Adds a device if its address is not already present.
    static bool addDevice(const BluetoothDevice& device,
                          const std::string& filepath);

    // Removes every device whose address matches.
    static bool removeDevice(const std::string& address,
                             const std::string& filepath);

};
