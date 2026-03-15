#pragma once

#include "IBluetoothManager.hpp"
#include "BluetoothDevice.hpp"
#include "../debugger/Debugger.hpp"
#include "../storage/DeviceStorage.hpp"
#include "../event/EventBus.hpp"
#include "../event/Event.hpp"

#include <chrono>
#include <thread>
#include <string>
#include <vector>

class MockBluetoothManager : public IBluetoothManager {
public:
    explicit MockBluetoothManager(EventBus& bus) : eventBus_(bus) {}
    ~MockBluetoothManager() override = default;

    MockBluetoothManager(const MockBluetoothManager&) = delete;
    MockBluetoothManager& operator=(const MockBluetoothManager&) = delete;

    bool powerOn() override;
    void shutdown() override;

    bool initialise() override;

    void discoverDevices() override;
    void abortDiscovery() override;

    bool pair(const std::string& mac) override;
    bool trust(const std::string& mac) override;
    bool connect(const std::string& mac) override;
    bool disconnect(const std::string& mac) override;

    bool pairAndSave(const BluetoothDevice& device) override;

    bool isConnected(const std::string& mac) override { return connected_; }

private:
    EventBus& eventBus_;
    bool connected_ = false;
    std::string connected_mac = "";
};