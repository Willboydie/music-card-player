#include "BluetoothManager.hpp"
#include "../storage/DeviceStorage.hpp"
#include "../ui/event/Event.hpp"
#include <cstring>
#include <iostream>


// ── Lifecycle ────────────────────────────────────────────────────────────────

BluetoothManager::BluetoothManager(EventBus& bus)
    : eventBus_(bus)
    , dbus(nullptr)
    , adapterPath("/org/bluez/hci0")
{}

BluetoothManager::~BluetoothManager() {
    shutdown();
}

bool BluetoothManager::initialise() {
    Debugger::debug_msg("BluetoothManager: initialising");
    GError* error = nullptr;
    dbus = g_bus_get_sync(G_BUS_TYPE_SYSTEM, nullptr, &error);
    if (error) {
        std::cerr << "BluetoothManager: D-Bus connection failed: "
                  << error->message << std::endl;
        g_error_free(error);
        return false;
    }
    Debugger::debug_msg("BluetoothManager: initialised");
    return true;
}

void BluetoothManager::shutdown() {
    Debugger::debug_msg("BluetoothManager: shutting down");
    if (dbus) {
        g_object_unref(dbus);
        dbus = nullptr;
    }
}


// ── Adapter ──────────────────────────────────────────────────────────────────

bool BluetoothManager::powerOn() {
    Debugger::debug_msg("BluetoothManager: powering on");
    return setProperty(adapterPath, "org.bluez.Adapter1",
                       "Powered", g_variant_new_boolean(TRUE));
}


// ── Discovery ────────────────────────────────────────────────────────────────

bool BluetoothManager::startDiscovery() {
    Debugger::debug_msg("BluetoothManager: starting discovery");
    return callMethod(adapterPath, "org.bluez.Adapter1", "StartDiscovery");
}

bool BluetoothManager::stopDiscovery() {
    Debugger::debug_msg("BluetoothManager: stopping discovery");
    return callMethod(adapterPath, "org.bluez.Adapter1", "StopDiscovery");
}

std::vector<BluetoothDevice> BluetoothManager::getDiscoveredDevices() {
    Debugger::debug_msg("BluetoothManager: getting discovered devices");
    std::vector<BluetoothDevice> devices;
    GError* error = nullptr;

    GVariant* result = g_dbus_connection_call_sync(
        dbus, "org.bluez", "/",
        "org.freedesktop.DBus.ObjectManager", "GetManagedObjects",
        nullptr,
        G_VARIANT_TYPE("(a{oa{sa{sv}}})"),
        G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error);

    if (error) {
        std::cerr << "BluetoothManager: getDiscoveredDevices failed: "
                  << error->message << std::endl;
        g_error_free(error);
        return devices;
    }

    GVariantIter* iter;
    const gchar* objectPath;
    GVariant* interfaces;

    g_variant_get(result, "(a{oa{sa{sv}}})", &iter);

    while (g_variant_iter_next(iter, "{&o@a{sa{sv}}}", &objectPath, &interfaces)) {
        if (std::strstr(objectPath, "/dev_") != nullptr) {
            GVariant* props = g_variant_lookup_value(
                interfaces, "org.bluez.Device1", nullptr);

            if (props) {
                BluetoothDevice device;
                device.name = "Unknown";

                GVariant* v = g_variant_lookup_value(props, "Name", nullptr);
                if (v) { device.name = g_variant_get_string(v, nullptr); g_variant_unref(v); }

                v = g_variant_lookup_value(props, "Address", nullptr);
                if (v) { device.address = g_variant_get_string(v, nullptr); g_variant_unref(v); }

                devices.push_back(std::move(device));
                g_variant_unref(props);
            }
        }
        g_variant_unref(interfaces);
    }

    g_variant_iter_free(iter);
    g_variant_unref(result);
    Debugger::debug_msg("BluetoothManager: got " + std::to_string(devices.size()) + " discovered devices");
    return devices;
}

std::vector<BluetoothDevice> BluetoothManager::completeDiscovery() {
    Debugger::debug_msg("BluetoothManager: completing discovery");
    stopDiscovery();
    auto devices = getDiscoveredDevices();
    DeviceStorage::save(devices, DeviceStorage::FOUND_DEVICES_FILE);
    for (const auto& device : devices) {
        eventBus_.publish(BluetoothDeviceDiscovered{device.name + " " + device.address});
    }
    eventBus_.publish(BluetoothDeviceSearchComplete{});
    Debugger::debug_msg("BluetoothManager: completed discovery");
    return devices;
}


// ── Device management ────────────────────────────────────────────────────────

bool BluetoothManager::pair(const std::string& mac) {
    Debugger::debug_msg("BluetoothManager: pairing " + mac);
    GError* error = nullptr;
    GVariant* result = g_dbus_connection_call_sync(
        dbus, "org.bluez", devicePath(mac).c_str(),
        "org.bluez.Device1", "Pair",
        nullptr, nullptr,
        G_DBUS_CALL_FLAGS_NONE, 30000, nullptr, &error);

    if (error) {
        // "Already Exists" is not a real failure
        if (std::strstr(error->message, "Already Exists") != nullptr) {
            g_error_free(error);
            return true;
        }
        std::cerr << "BluetoothManager: pair failed: "
                  << error->message << std::endl;
        g_error_free(error);
        return false;
    }

    if (result) g_variant_unref(result);
    Debugger::debug_msg("BluetoothManager: paired " + mac);
    return true;
}

bool BluetoothManager::pairAndSave(const BluetoothDevice& device) {
    Debugger::debug_msg("BluetoothManager: pairing and saving " + device.address);
    if (!pair(device.address)) return false;
    if (!trust(device.address)) return false;
    DeviceStorage::addDevice(device, DeviceStorage::SAVED_DEVICES_FILE);
    Debugger::debug_msg("BluetoothManager: paired and saved " + device.address);
    return true;
}

bool BluetoothManager::trust(const std::string& mac) {
    Debugger::debug_msg("BluetoothManager: trusting " + mac);
    return setProperty(devicePath(mac), "org.bluez.Device1",
                       "Trusted", g_variant_new_boolean(TRUE));
}

bool BluetoothManager::connect(const std::string& mac) {
    Debugger::debug_msg("BluetoothManager: connecting " + mac);
    bool ok = callMethod(devicePath(mac), "org.bluez.Device1",
                         "Connect", nullptr, 30000);
    if (ok) {
        eventBus_.publish(BluetoothConnected{});
    } else {
        eventBus_.publish(BluetoothConnectionFailed{});
    }
    return ok;
}

bool BluetoothManager::disconnect(const std::string& mac) {
    Debugger::debug_msg("BluetoothManager: disconnecting " + mac);
    return callMethod(devicePath(mac), "org.bluez.Device1", "Disconnect");
}


// ── Stored device access ─────────────────────────────────────────────────────

std::vector<BluetoothDevice> BluetoothManager::getSavedDevices() const {
    return DeviceStorage::load(DeviceStorage::SAVED_DEVICES_FILE);
}

std::vector<BluetoothDevice> BluetoothManager::getFoundDevices() const {
    return DeviceStorage::load(DeviceStorage::FOUND_DEVICES_FILE);
}


// ── Status ───────────────────────────────────────────────────────────────────

bool BluetoothManager::isConnected(const std::string& mac) {
    GVariant* value = getProperty(devicePath(mac),
                                  "org.bluez.Device1", "Connected");
    if (!value) return false;

    bool connected = g_variant_get_boolean(value);
    g_variant_unref(value);
    return connected;
}


// ── D-Bus helpers ────────────────────────────────────────────────────────────

std::string BluetoothManager::devicePath(const std::string& mac) const {
    std::string path = adapterPath + "/dev_";
    for (char c : mac) {
        path += (c == ':') ? '_' : c;
    }
    return path;
}

bool BluetoothManager::callMethod(const std::string& objectPath,
                                   const std::string& interface,
                                   const std::string& method,
                                   GVariant* params,
                                   int timeoutMs) {
    GError* error = nullptr;
    GVariant* result = g_dbus_connection_call_sync(
        dbus, "org.bluez",
        objectPath.c_str(), interface.c_str(), method.c_str(),
        params, nullptr,
        G_DBUS_CALL_FLAGS_NONE, timeoutMs, nullptr, &error);

    if (error) {
        std::cerr << "BluetoothManager: " << method << " failed: "
                  << error->message << std::endl;
        g_error_free(error);
        return false;
    }

    if (result) g_variant_unref(result);
    return true;
}

bool BluetoothManager::setProperty(const std::string& objectPath,
                                    const std::string& interface,
                                    const std::string& property,
                                    GVariant* value) {
    GError* error = nullptr;
    GVariant* result = g_dbus_connection_call_sync(
        dbus, "org.bluez",
        objectPath.c_str(),
        "org.freedesktop.DBus.Properties", "Set",
        g_variant_new("(ssv)", interface.c_str(), property.c_str(), value),
        nullptr,
        G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error);

    if (error) {
        std::cerr << "BluetoothManager: set " << property << " failed: "
                  << error->message << std::endl;
        g_error_free(error);
        return false;
    }

    if (result) g_variant_unref(result);
    return true;
}

GVariant* BluetoothManager::getProperty(const std::string& objectPath,
                                         const std::string& interface,
                                         const std::string& property) {
    GError* error = nullptr;
    GVariant* result = g_dbus_connection_call_sync(
        dbus, "org.bluez",
        objectPath.c_str(),
        "org.freedesktop.DBus.Properties", "Get",
        g_variant_new("(ss)", interface.c_str(), property.c_str()),
        G_VARIANT_TYPE("(v)"),
        G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error);

    if (error) {
        g_error_free(error);
        return nullptr;
    }

    GVariant* inner = nullptr;
    g_variant_get(result, "(v)", &inner);
    g_variant_unref(result);
    return inner;   // caller owns
}
