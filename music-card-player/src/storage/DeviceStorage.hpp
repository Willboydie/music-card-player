#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

struct SavedDevice {
    std::string name;
    std::string address;  // MAC address
    
    SavedDevice(const std::string& _name, const std::string& _address)
        : name(_name), address(_address) {}
};

class DeviceStorage {
public:
    static constexpr const char* DEFAULT_FILE = "saved_devices.txt";
    
    // Load saved devices from file
    static std::vector<SavedDevice> loadDevices(const std::string& filename = DEFAULT_FILE) {
        std::vector<SavedDevice> devices;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            // File doesn't exist yet, return empty list
            return devices;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;  // Skip empty lines and comments
            }
            
            // Format: "Device Name:AA:BB:CC:DD:EE:FF"
            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                std::string name = line.substr(0, colon_pos);
                std::string address = line.substr(colon_pos + 1);
                
                // Trim whitespace
                name.erase(0, name.find_first_not_of(" \t"));
                name.erase(name.find_last_not_of(" \t") + 1);
                address.erase(0, address.find_first_not_of(" \t"));
                address.erase(address.find_last_not_of(" \t") + 1);
                
                if (!name.empty() && !address.empty()) {
                    devices.emplace_back(name, address);
                }
            }
        }
        
        return devices;
    }
    
    // Save devices to file
    static bool saveDevices(const std::vector<SavedDevice>& devices, 
                            const std::string& filename = DEFAULT_FILE) {
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            return false;
        }
        
        file << "# Saved Bluetooth Devices\n";
        file << "# Format: Device Name:MAC:Address\n";
        
        for (const auto& device : devices) {
            file << device.name << ":" << device.address << "\n";
        }
        
        return true;
    }
    
    // Add a device (loads, adds, saves)
    static bool addDevice(const std::string& name, const std::string& address,
                         const std::string& filename = DEFAULT_FILE) {
        auto devices = loadDevices(filename);
        
        // Check if device already exists
        for (const auto& device : devices) {
            if (device.address == address) {
                return false;  // Device already exists
            }
        }
        
        devices.emplace_back(name, address);
        return saveDevices(devices, filename);
    }
    
    // Remove a device by address
    static bool removeDevice(const std::string& address,
                            const std::string& filename = DEFAULT_FILE) {
        auto devices = loadDevices(filename);
        
        devices.erase(
            std::remove_if(devices.begin(), devices.end(),
                [&address](const SavedDevice& d) { return d.address == address; }),
            devices.end()
        );
        
        return saveDevices(devices, filename);
    }
};
