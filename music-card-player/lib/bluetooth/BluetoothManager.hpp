#pragma once

#include "IBluetoothManager.hpp"
#include "BluetoothDevice.hpp"
#include "../debugger/Debugger.hpp"
#include "../storage/DeviceStorage.hpp"
#include "../event/EventBus.hpp"

#include <cstring>
#include <iostream>
#include <gio/gio.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

static constexpr int BT_MAX_RETRIES = 12;
static constexpr auto BT_RETRY_INTERVAL = std::chrono::seconds(5);

class BluetoothManager : public IBluetoothManager {
public:
    explicit BluetoothManager(EventBus& bus);
    ~BluetoothManager() override;

    BluetoothManager(const BluetoothManager&) = delete;
    BluetoothManager& operator=(const BluetoothManager&) = delete;

    bool powerOn() override;
    void shutdown() override;

    bool initialise() override;

    void discoverDevices() override;
    void abortDiscovery() override;

    
    bool connect(const std::string& mac) override;
    bool disconnect(const std::string& mac) override;
    bool pairAndSave(const BluetoothDevice& device) override;
    bool isConnected(const std::string& mac) override;

private:
    EventBus& eventBus_;
    GDBusConnection* dbus;
    std::string adapterPath;
    guint agentRegistrationId = 0;

    bool registerAgent();
    void unregisterAgent();

    bool adapterReady();

    bool startDiscovery();
    bool stopDiscovery();
    std::vector<BluetoothDevice> getDiscoveredDevices();
    
    bool pair(const std::string& mac) override;
    bool trust(const std::string& mac) override;

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
