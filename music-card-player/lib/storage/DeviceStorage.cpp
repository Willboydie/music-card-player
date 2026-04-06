#include "DeviceStorage.hpp"

#include <cstdlib>
#include <filesystem>

static constexpr char Delimiter = '|';

namespace {

std::string resolveDataPath(const std::string& filepath) {
    if (!filepath.empty() && filepath[0] == '/')
        return filepath;
    const char* root = std::getenv("MUSIC_CARD_PLAYER_HOME");
    if (root && root[0]) {
        std::string base(root);
        if (base.back() != '/') base += '/';
        return base + filepath;
    }
    return filepath;
}

std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

} // namespace

// ── Bulk I/O ─────────────────────────────────────────────────────────────────

std::vector<BluetoothDevice> DeviceStorage::load(const std::string& filepath) {
    const std::string path = resolveDataPath(filepath);
    Debugger::debug_msg("DeviceStorage: loading " + path);
    std::vector<BluetoothDevice> devices;
    std::ifstream file(path);
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

    Debugger::debug_msg("DeviceStorage: loaded " + std::to_string(devices.size()) + " devices");
    return devices;
}

bool DeviceStorage::save(const std::vector<BluetoothDevice>& devices,
                          const std::string& filepath) {
    const std::string path = resolveDataPath(filepath);
    Debugger::debug_msg("DeviceStorage: saving " + path);
    {
        const std::filesystem::path parent = std::filesystem::path(path).parent_path();
        if (!parent.empty()) {
            std::error_code ec;
            std::filesystem::create_directories(parent, ec);
        }
    }
    std::ofstream file(path);
    if (!file.is_open()) return false;

    file << "# Bluetooth devices – name|MAC\n";
    for (const auto& d : devices) {
        file << d.name << Delimiter << d.address << '\n';
    }

    Debugger::debug_msg("DeviceStorage: saved " + std::to_string(devices.size()) + " devices");
    return file.good();
}


// ── Lookup ───────────────────────────────────────────────────────────────────

std::string DeviceStorage::findAddressByName(const std::string& name,
                                              const std::string& filepath) {
    Debugger::debug_msg("DeviceStorage: finding address by name " + name + " in " + filepath);
    auto devices = load(filepath);
    for (const auto& d : devices) {
        if (d.name == name) return d.address;
    }
    Debugger::debug_msg("DeviceStorage: found address by name " + name + " in " + filepath);
    return {};
}


// ── Convenience helpers ──────────────────────────────────────────────────────

bool DeviceStorage::addDevice(const BluetoothDevice& device,
                               const std::string& filepath) {
    Debugger::debug_msg("DeviceStorage: adding device " + device.address + " to " + filepath);
    auto devices = load(filepath);

    // Don't duplicate by address
    for (const auto& d : devices) {
        if (d.address == device.address) return true;  // already present
    }

    devices.push_back(device);
    Debugger::debug_msg("DeviceStorage: added device " + device.address + " to " + filepath);
    bool result = save(devices, filepath);
    Debugger::debug_msg("DeviceStorage: saved " + std::to_string(devices.size()) + " devices: " + (result ? "success" : "failure"));
    return result;
}

bool DeviceStorage::removeDevice(const std::string& address,
                                  const std::string& filepath) {
    Debugger::debug_msg("DeviceStorage: removing device " + address + " from " + filepath);
    auto devices = load(filepath);

    devices.erase(
        std::remove_if(devices.begin(), devices.end(),
            [&address](const BluetoothDevice& d) { return d.address == address; }),
        devices.end());

    Debugger::debug_msg("DeviceStorage: removed device " + address + " from " + filepath);
    bool result = save(devices, filepath);
    Debugger::debug_msg("DeviceStorage: saved " + std::to_string(devices.size()) + " devices: " + (result ? "success" : "failure"));
    return result;
}
