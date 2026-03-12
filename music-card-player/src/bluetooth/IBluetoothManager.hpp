#pragma once

#include "BluetoothDevice.hpp"
#include <string>
#include <vector>

class IBluetoothManager {
public:
    virtual ~IBluetoothManager() = default;

    // ── Lifecycle ────────────────────────────────────────────────
    virtual bool initialise() = 0;
    virtual void shutdown() = 0;

    // ── Adapter ──────────────────────────────────────────────────
    virtual bool powerOn() = 0;

    // ── Discovery ────────────────────────────────────────────────
    virtual void discoverDevices() = 0;

    // ── Device management ────────────────────────────────────────
    virtual bool pair(const std::string& mac) = 0;
    virtual bool trust(const std::string& mac) = 0;
    virtual bool connect(const std::string& mac) = 0;
    virtual bool disconnect(const std::string& mac) = 0;
    virtual bool pairAndSave(const BluetoothDevice& device) = 0;

    // ── Stored device access ─────────────────────────────────────
    virtual std::vector<BluetoothDevice> getSavedDevices() const = 0;
    virtual std::vector<BluetoothDevice> getFoundDevices() const = 0;

    // ── Status ───────────────────────────────────────────────────
    virtual bool isConnected(const std::string& mac) = 0;
};
