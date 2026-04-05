#include "BluetoothManager.hpp"


// ── Agent ────────────────────────────────────────────────────────────────────

static const char* AGENT_PATH = "/org/bluez/AutoAgent";

static const gchar agentIntrospectionXml[] =
    "<node>"
    "  <interface name='org.bluez.Agent1'>"
    "    <method name='Release'/>"
    "    <method name='RequestPinCode'>"
    "      <arg type='o' name='device' direction='in'/>"
    "      <arg type='s' name='pincode' direction='out'/>"
    "    </method>"
    "    <method name='DisplayPinCode'>"
    "      <arg type='o' name='device' direction='in'/>"
    "      <arg type='s' name='pincode' direction='in'/>"
    "    </method>"
    "    <method name='RequestPasskey'>"
    "      <arg type='o' name='device' direction='in'/>"
    "      <arg type='u' name='passkey' direction='out'/>"
    "    </method>"
    "    <method name='DisplayPasskey'>"
    "      <arg type='o' name='device' direction='in'/>"
    "      <arg type='u' name='passkey' direction='in'/>"
    "      <arg type='q' name='entered' direction='in'/>"
    "    </method>"
    "    <method name='RequestConfirmation'>"
    "      <arg type='o' name='device' direction='in'/>"
    "      <arg type='u' name='passkey' direction='in'/>"
    "    </method>"
    "    <method name='RequestAuthorization'>"
    "      <arg type='o' name='device' direction='in'/>"
    "    </method>"
    "    <method name='AuthorizeService'>"
    "      <arg type='o' name='device' direction='in'/>"
    "      <arg type='s' name='uuid' direction='in'/>"
    "    </method>"
    "    <method name='Cancel'/>"
    "  </interface>"
    "</node>";

static void agentMethodCall(GDBusConnection*,
                            const gchar*,
                            const gchar*,
                            const gchar*,
                            const gchar* method,
                            GVariant*,
                            GDBusMethodInvocation* invocation,
                            gpointer) {
    Debugger::debug_msg(std::string("BluetoothAgent: ") + method);

    if (g_strcmp0(method, "RequestPinCode") == 0) {
        g_dbus_method_invocation_return_value(
            invocation, g_variant_new("(s)", "0000"));
    } else if (g_strcmp0(method, "RequestPasskey") == 0) {
        g_dbus_method_invocation_return_value(
            invocation, g_variant_new("(u)", 0));
    } else if (g_strcmp0(method, "RequestConfirmation") == 0 ||
               g_strcmp0(method, "RequestAuthorization") == 0 ||
               g_strcmp0(method, "AuthorizeService") == 0 ||
               g_strcmp0(method, "DisplayPinCode") == 0 ||
               g_strcmp0(method, "DisplayPasskey") == 0 ||
               g_strcmp0(method, "Release") == 0 ||
               g_strcmp0(method, "Cancel") == 0) {
        g_dbus_method_invocation_return_value(invocation, nullptr);
    } else {
        g_dbus_method_invocation_return_dbus_error(
            invocation, "org.bluez.Error.Rejected", "Unknown method");
    }
}

static const GDBusInterfaceVTable agentVtable = {
    agentMethodCall, nullptr, nullptr, { 0 }
};

bool BluetoothManager::registerAgent() {
    Debugger::debug_msg("BluetoothManager: registering agent");

    GDBusNodeInfo* nodeInfo =
        g_dbus_node_info_new_for_xml(agentIntrospectionXml, nullptr);
    if (!nodeInfo) {
        std::cerr << "BluetoothManager: failed to parse agent introspection XML"
                  << std::endl;
        return false;
    }

    GError* error = nullptr;
    agentRegistrationId = g_dbus_connection_register_object(
        dbus, AGENT_PATH,
        nodeInfo->interfaces[0],
        &agentVtable,
        nullptr, nullptr, &error);
    g_dbus_node_info_unref(nodeInfo);

    if (error) {
        std::cerr << "BluetoothManager: agent object registration failed: "
                  << error->message << std::endl;
        g_error_free(error);
        return false;
    }

    GVariant* result = g_dbus_connection_call_sync(
        dbus, "org.bluez", "/org/bluez",
        "org.bluez.AgentManager1", "RegisterAgent",
        g_variant_new("(os)", AGENT_PATH, "NoInputNoOutput"),
        nullptr, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error);

    if (error) {
        std::cerr << "BluetoothManager: RegisterAgent failed: "
                  << error->message << std::endl;
        g_error_free(error);
        return false;
    }
    if (result) g_variant_unref(result);

    result = g_dbus_connection_call_sync(
        dbus, "org.bluez", "/org/bluez",
        "org.bluez.AgentManager1", "RequestDefaultAgent",
        g_variant_new("(o)", AGENT_PATH),
        nullptr, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error);

    if (error) {
        std::cerr << "BluetoothManager: RequestDefaultAgent failed: "
                  << error->message << std::endl;
        g_error_free(error);
        return false;
    }
    if (result) g_variant_unref(result);

    Debugger::debug_msg("BluetoothManager: agent registered");
    return true;
}

void BluetoothManager::unregisterAgent() {
    if (!dbus) return;

    GError* error = nullptr;
    GVariant* result = g_dbus_connection_call_sync(
        dbus, "org.bluez", "/org/bluez",
        "org.bluez.AgentManager1", "UnregisterAgent",
        g_variant_new("(o)", AGENT_PATH),
        nullptr, G_DBUS_CALL_FLAGS_NONE, -1, nullptr, &error);

    if (error) g_error_free(error);
    if (result) g_variant_unref(result);

    if (agentRegistrationId > 0) {
        g_dbus_connection_unregister_object(dbus, agentRegistrationId);
        agentRegistrationId = 0;
    }
}


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

    if (!registerAgent()) {
        std::cerr << "BluetoothManager: agent registration failed, "
                     "pairing may not work" << std::endl;
    }

    Debugger::debug_msg("BluetoothManager: initialised");
    return true;
}

void BluetoothManager::shutdown() {
    Debugger::debug_msg("BluetoothManager: shutting down");
    if (dbus) {
        unregisterAgent();
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

void BluetoothManager::discoverDevices() {
    if (!startDiscovery()) {
        eventBus_.publish(BluetoothDiscoveryFailed{});
        return;
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
    if (!stopDiscovery()) {
        eventBus_.publish(BluetoothDiscoveryFailed{});
        return;
    }
    auto devices = getDiscoveredDevices();
    DeviceStorage::save(devices, DeviceStorage::FOUND_DEVICES_FILE);
    eventBus_.publish(BluetoothDeviceSearchComplete{});
    return;
}

void BluetoothManager::abortDiscovery() {
    Debugger::debug_msg("BluetoothManager: aborting discovery");
    stopDiscovery();
    return;
}


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
                if (v) {
                    std::string name = g_variant_get_string(v, nullptr);
                    g_variant_unref(v);
                    if (!name.empty()) device.name = std::move(name);
                }
                v = g_variant_lookup_value(props, "Alias", nullptr);
                if (v && device.name == "Unknown") {
                    std::string alias = g_variant_get_string(v, nullptr);
                    g_variant_unref(v);
                    if (!alias.empty()) device.name = std::move(alias);
                }

                v = g_variant_lookup_value(props, "Address", nullptr);
                if (v) { device.address = g_variant_get_string(v, nullptr); g_variant_unref(v); }

                bool isAudio = false;

                // Check Class of Device (CoD): major device class is bits 8-12
                v = g_variant_lookup_value(props, "Class", nullptr);
                if (v) {
                    guint32 deviceClass = g_variant_get_uint32(v);
                    g_variant_unref(v);
                    guint32 majorClass = (deviceClass >> 8) & 0x1F;
                    if (majorClass == 0x04) isAudio = true;
                }

                // Fallback: check UUIDs for common audio profiles
                if (!isAudio) {
                    v = g_variant_lookup_value(props, "UUIDs", nullptr);
                    if (v) {
                        GVariantIter uuidIter;
                        g_variant_iter_init(&uuidIter, v);
                        const gchar* uuid;
                        while (g_variant_iter_next(&uuidIter, "&s", &uuid)) {
                            std::string u(uuid);
                            if (u.size() >= 8) {
                                std::string prefix = u.substr(0, 8);
                                for (auto& c : prefix) c = std::tolower(c);
                                if (prefix == "0000110a" ||
                                    prefix == "0000110b" ||
                                    prefix == "0000110d" ||
                                    prefix == "0000110e" ||
                                    prefix == "00001108" ||
                                    prefix == "0000111e") {
                                    isAudio = true;
                                    break;
                                }
                            }
                        }
                        g_variant_unref(v);
                    }
                }

                if (isAudio) {
                    devices.push_back(std::move(device));
                }
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


// ── Device connection ────────────────────────────────────────────────────────

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
