#include "DeviceStorage.hpp"
#include <fstream>
#include <algorithm>

static constexpr char Delimiter = '|';


// ── Helpers ──────────────────────────────────────────────────────────────────

static std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}


// ── Bulk I/O ─────────────────────────────────────────────────────────────────

std::vector<BluetoothDevice> DeviceStorage::load(const std::string& filepath) {
    std::vector<BluetoothDevice> devices;
    std::ifstream file(filepath);
    if (!file.is_open()) return devices;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        auto pos = line.find(Delimiter);
        if (pos == std::string::npos) continue;

        std::string name    = trim(line.substr(0, pos));
        std::string address = trim(line.substr(pos + 1));

        if (!name.empty() && !address.empty()) {
            devices.push_back({ std::move(address), std::move(name) });
        }
    }

    return devices;
}

bool DeviceStorage::save(const std::vector<BluetoothDevice>& devices,
                          const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) return false;

    file << "# Bluetooth devices – name|MAC\n";
    for (const auto& d : devices) {
        file << d.name << Delimiter << d.address << '\n';
    }

    return file.good();
}


// ── Lookup ───────────────────────────────────────────────────────────────────

std::string DeviceStorage::findAddressByName(const std::string& name,
                                              const std::string& filepath) {
    auto devices = load(filepath);
    for (const auto& d : devices) {
        if (d.name == name) return d.address;
    }
    return {};
}


// ── Convenience helpers ──────────────────────────────────────────────────────

bool DeviceStorage::addDevice(const BluetoothDevice& device,
                               const std::string& filepath) {
    auto devices = load(filepath);

    // Don't duplicate by address
    for (const auto& d : devices) {
        if (d.address == device.address) return true;  // already present
    }

    devices.push_back(device);
    return save(devices, filepath);
}

bool DeviceStorage::removeDevice(const std::string& address,
                                  const std::string& filepath) {
    auto devices = load(filepath);

    devices.erase(
        std::remove_if(devices.begin(), devices.end(),
            [&address](const BluetoothDevice& d) { return d.address == address; }),
        devices.end());

    return save(devices, filepath);
}
