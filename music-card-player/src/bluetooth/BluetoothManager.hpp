#pragma once

#include "IBluetoothManager.hpp"
#include "BluetoothDevice.hpp"
#include <gio/gio.h>
#include <string>
#include <vector>
#include "../debugger/Debugger.hpp"
#include "../storage/DeviceStorage.hpp"
#include "../ui/event/EventBus.hpp"
#include <cstring>
#include <iostream>

class BluetoothManager : public IBluetoothManager {
public:
    explicit BluetoothManager(EventBus& bus);
    ~BluetoothManager() override;

    BluetoothManager(const BluetoothManager&) = delete;
    BluetoothManager& operator=(const BluetoothManager&) = delete;

    bool initialise() override;
    void shutdown() override;
    bool powerOn() override;
    bool startDiscovery() override;
    bool stopDiscovery() override;
    std::vector<BluetoothDevice> getDiscoveredDevices() override;
    std::vector<BluetoothDevice> completeDiscovery() override;
    bool pair(const std::string& mac) override;
    bool trust(const std::string& mac) override;
    bool connect(const std::string& mac) override;
    bool disconnect(const std::string& mac) override;
    bool pairAndSave(const BluetoothDevice& device) override;
    std::vector<BluetoothDevice> getSavedDevices() const override;
    std::vector<BluetoothDevice> getFoundDevices() const override;
    bool isConnected(const std::string& mac) override;

private:
    EventBus& eventBus_;
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
