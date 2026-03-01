#pragma once

#include "IBluetoothManager.hpp"
#include "BluetoothDevice.hpp"
#include <string>
#include <vector>
#include "../debugger/Debugger.hpp"
#include "../storage/DeviceStorage.hpp"
#include "../ui/event/EventBus.hpp"
#include "../ui/event/Event.hpp"

#include <chrono>
#include <thread>

class MockBluetoothManager : public IBluetoothManager {
public:
    explicit MockBluetoothManager(EventBus& bus) : eventBus_(bus) {}
    ~MockBluetoothManager() override = default;

    MockBluetoothManager(const MockBluetoothManager&) = delete;
    MockBluetoothManager& operator=(const MockBluetoothManager&) = delete;

    bool initialise() override { 
        Debugger::debug_msg("MockBluetoothManager: initialised");
        return true; 
    }
    void shutdown() override {
        Debugger::debug_msg("MockBluetoothManager: shutting down");
    }

    bool powerOn() override {
        Debugger::debug_msg("MockBluetoothManager: powered on");
        return true;
    }

    bool startDiscovery() override {
        Debugger::debug_msg("MockBluetoothManager: started discovery");
        return true;
    }
    bool stopDiscovery() override {
        Debugger::debug_msg("MockBluetoothManager: stopped discovery");
        return true;
    }

    std::vector<BluetoothDevice> getDiscoveredDevices() override {
        Debugger::debug_msg("MockBluetoothManager: getting discovered devices...");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        Debugger::debug_msg("MockBluetoothManager: discovered " + std::to_string(DeviceStorage::load(DeviceStorage::MOCK_DEVICES_FILE).size()) + " devices");
        return DeviceStorage::load(DeviceStorage::MOCK_DEVICES_FILE);
    }

    std::vector<BluetoothDevice> completeDiscovery() override {
        Debugger::debug_msg("MockBluetoothManager: completing discovery");
        auto devices = DeviceStorage::load(DeviceStorage::MOCK_DEVICES_FILE);
        for (const auto& device : devices) {
            eventBus_.publish(BluetoothDeviceDiscovered{device.name + " " + device.address});
        }
        eventBus_.publish(BluetoothDeviceSearchComplete{});
        return devices;
    }

    bool pair(const std::string& mac) override {
        Debugger::debug_msg("MockBluetoothManager: pairing " + mac);
        return true;
    }
    bool trust(const std::string& mac) override {
        Debugger::debug_msg("MockBluetoothManager: trusting " + mac);
        return true;
    }
    bool connect(const std::string& mac) override {
        Debugger::debug_msg("MockBluetoothManager: connecting... " + mac);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        connected_ = true;
        connected_mac = mac;
        eventBus_.publish(BluetoothConnected{});
        return true;
    }
    bool disconnect(const std::string& mac) override {
        Debugger::debug_msg("MockBluetoothManager: disconnected device " + mac);
        connected_ = false;
        return true;
    }

    bool pairAndSave(const BluetoothDevice& device) override {
        Debugger::debug_msg("MockBluetoothManager: pairing and saving " + device.address);
        return true;
    }

    std::vector<BluetoothDevice> getSavedDevices() const override {
        Debugger::debug_msg("MockBluetoothManager: getting saved devices");
        return DeviceStorage::load(DeviceStorage::MOCK_DEVICES_FILE);
    }
    std::vector<BluetoothDevice> getFoundDevices() const override {
        Debugger::debug_msg("MockBluetoothManager: getting found devices");
        return DeviceStorage::load(DeviceStorage::MOCK_DEVICES_FILE);
    }

    bool isConnected(const std::string& mac) override { return connected_; }

private:
    EventBus& eventBus_;
    bool connected_ = false;
    std::string connected_mac = "";
};