/**
 * Bluetooth Audio Connection for Raspberry Pi Zero 2 W
 * 
 * This boilerplate uses BlueZ D-Bus API to:
 * - Scan for Bluetooth devices
 * - Pair with a device
 * - Connect to Bluetooth headphones
 * 
 * Build: g++ -o bluetooth_audio bluetooth_audio.cpp $(pkg-config --cflags --libs gio-2.0)
 */

#include <gio/gio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

class BluetoothManager {
private:
    GDBusConnection* connection;
    GMainLoop* loop;
    std::string adapter_path;
    
public:
    BluetoothManager() : connection(nullptr), loop(nullptr), adapter_path("/org/bluez/hci0") {}
    
    ~BluetoothManager() {
        if (connection) {
            g_object_unref(connection);
        }
        if (loop) {
            g_main_loop_unref(loop);
        }
    }
    
    bool initialize() {
        GError* error = nullptr;
        
        // Connect to system D-Bus
        connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, nullptr, &error);
        if (error) {
            std::cerr << "Failed to connect to D-Bus: " << error->message << std::endl;
            g_error_free(error);
            return false;
        }
        
        std::cout << "Connected to D-Bus successfully" << std::endl;
        return true;
    }
    
    bool powerOnAdapter() {
        GError* error = nullptr;
        GVariant* result;
        
        // Set Powered property to true
        result = g_dbus_connection_call_sync(
            connection,
            "org.bluez",
            adapter_path.c_str(),
            "org.freedesktop.DBus.Properties",
            "Set",
            g_variant_new("(ssv)", "org.bluez.Adapter1", "Powered", g_variant_new_boolean(TRUE)),
            nullptr,
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            nullptr,
            &error
        );
        
        if (error) {
            std::cerr << "Failed to power on adapter: " << error->message << std::endl;
            g_error_free(error);
            return false;
        }
        
        if (result) g_variant_unref(result);
        std::cout << "Bluetooth adapter powered on" << std::endl;
        return true;
    }
    
    bool startDiscovery() {
        GError* error = nullptr;
        GVariant* result;
        
        result = g_dbus_connection_call_sync(
            connection,
            "org.bluez",
            adapter_path.c_str(),
            "org.bluez.Adapter1",
            "StartDiscovery",
            nullptr,
            nullptr,
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            nullptr,
            &error
        );
        
        if (error) {
            std::cerr << "Failed to start discovery: " << error->message << std::endl;
            g_error_free(error);
            return false;
        }
        
        if (result) g_variant_unref(result);
        std::cout << "Discovery started - put your headphones in pairing mode" << std::endl;
        return true;
    }
    
    bool stopDiscovery() {
        GError* error = nullptr;
        GVariant* result;
        
        result = g_dbus_connection_call_sync(
            connection,
            "org.bluez",
            adapter_path.c_str(),
            "org.bluez.Adapter1",
            "StopDiscovery",
            nullptr,
            nullptr,
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            nullptr,
            &error
        );
        
        if (error) {
            std::cerr << "Failed to stop discovery: " << error->message << std::endl;
            g_error_free(error);
            return false;
        }
        
        if (result) g_variant_unref(result);
        std::cout << "Discovery stopped" << std::endl;
        return true;
    }
    
    std::vector<std::pair<std::string, std::string>> getDiscoveredDevices() {
        std::vector<std::pair<std::string, std::string>> devices;
        GError* error = nullptr;
        GVariant* result;
        
        // Get managed objects from BlueZ
        result = g_dbus_connection_call_sync(
            connection,
            "org.bluez",
            "/",
            "org.freedesktop.DBus.ObjectManager",
            "GetManagedObjects",
            nullptr,
            G_VARIANT_TYPE("(a{oa{sa{sv}}})"),
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            nullptr,
            &error
        );
        
        if (error) {
            std::cerr << "Failed to get devices: " << error->message << std::endl;
            g_error_free(error);
            return devices;
        }
        
        GVariantIter* iter;
        const gchar* object_path;
        GVariant* interfaces;
        
        g_variant_get(result, "(a{oa{sa{sv}}})", &iter);
        
        while (g_variant_iter_next(iter, "{&o@a{sa{sv}}}", &object_path, &interfaces)) {
            // Check if this is a device (path contains /dev_)
            if (strstr(object_path, "/dev_") != nullptr) {
                GVariant* device_props = g_variant_lookup_value(interfaces, "org.bluez.Device1", nullptr);
                
                if (device_props) {
                    GVariant* name_variant = g_variant_lookup_value(device_props, "Name", nullptr);
                    GVariant* address_variant = g_variant_lookup_value(device_props, "Address", nullptr);
                    
                    std::string name = "Unknown";
                    std::string address = "";
                    
                    if (name_variant) {
                        name = g_variant_get_string(name_variant, nullptr);
                        g_variant_unref(name_variant);
                    }
                    if (address_variant) {
                        address = g_variant_get_string(address_variant, nullptr);
                        g_variant_unref(address_variant);
                    }
                    
                    devices.push_back({address, name});
                    g_variant_unref(device_props);
                }
            }
            g_variant_unref(interfaces);
        }
        
        g_variant_iter_free(iter);
        g_variant_unref(result);
        
        return devices;
    }
    
    std::string getDevicePath(const std::string& mac_address) {
        // Convert MAC address to BlueZ device path format
        // e.g., "AA:BB:CC:DD:EE:FF" -> "/org/bluez/hci0/dev_AA_BB_CC_DD_EE_FF"
        std::string path = adapter_path + "/dev_";
        for (char c : mac_address) {
            path += (c == ':') ? '_' : c;
        }
        return path;
    }
    
    bool pairDevice(const std::string& mac_address) {
        GError* error = nullptr;
        GVariant* result;
        std::string device_path = getDevicePath(mac_address);
        
        std::cout << "Pairing with device: " << mac_address << std::endl;
        
        result = g_dbus_connection_call_sync(
            connection,
            "org.bluez",
            device_path.c_str(),
            "org.bluez.Device1",
            "Pair",
            nullptr,
            nullptr,
            G_DBUS_CALL_FLAGS_NONE,
            30000,  // 30 second timeout for pairing
            nullptr,
            &error
        );
        
        if (error) {
            // Already paired is not really an error
            if (strstr(error->message, "Already Exists") != nullptr) {
                std::cout << "Device already paired" << std::endl;
                g_error_free(error);
                return true;
            }
            std::cerr << "Failed to pair: " << error->message << std::endl;
            g_error_free(error);
            return false;
        }
        
        if (result) g_variant_unref(result);
        std::cout << "Pairing successful" << std::endl;
        return true;
    }
    
    bool trustDevice(const std::string& mac_address) {
        GError* error = nullptr;
        GVariant* result;
        std::string device_path = getDevicePath(mac_address);
        
        result = g_dbus_connection_call_sync(
            connection,
            "org.bluez",
            device_path.c_str(),
            "org.freedesktop.DBus.Properties",
            "Set",
            g_variant_new("(ssv)", "org.bluez.Device1", "Trusted", g_variant_new_boolean(TRUE)),
            nullptr,
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            nullptr,
            &error
        );
        
        if (error) {
            std::cerr << "Failed to trust device: " << error->message << std::endl;
            g_error_free(error);
            return false;
        }
        
        if (result) g_variant_unref(result);
        std::cout << "Device trusted" << std::endl;
        return true;
    }
    
    bool connectDevice(const std::string& mac_address) {
        GError* error = nullptr;
        GVariant* result;
        std::string device_path = getDevicePath(mac_address);
        
        std::cout << "Connecting to device: " << mac_address << std::endl;
        
        result = g_dbus_connection_call_sync(
            connection,
            "org.bluez",
            device_path.c_str(),
            "org.bluez.Device1",
            "Connect",
            nullptr,
            nullptr,
            G_DBUS_CALL_FLAGS_NONE,
            30000,  // 30 second timeout
            nullptr,
            &error
        );
        
        if (error) {
            std::cerr << "Failed to connect: " << error->message << std::endl;
            g_error_free(error);
            return false;
        }
        
        if (result) g_variant_unref(result);
        std::cout << "Connected successfully!" << std::endl;
        return true;
    }
    
    bool disconnectDevice(const std::string& mac_address) {
        GError* error = nullptr;
        GVariant* result;
        std::string device_path = getDevicePath(mac_address);
        
        result = g_dbus_connection_call_sync(
            connection,
            "org.bluez",
            device_path.c_str(),
            "org.bluez.Device1",
            "Disconnect",
            nullptr,
            nullptr,
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            nullptr,
            &error
        );
        
        if (error) {
            std::cerr << "Failed to disconnect: " << error->message << std::endl;
            g_error_free(error);
            return false;
        }
        
        if (result) g_variant_unref(result);
        std::cout << "Disconnected" << std::endl;
        return true;
    }
    
    bool isDeviceConnected(const std::string& mac_address) {
        GError* error = nullptr;
        GVariant* result;
        std::string device_path = getDevicePath(mac_address);
        
        result = g_dbus_connection_call_sync(
            connection,
            "org.bluez",
            device_path.c_str(),
            "org.freedesktop.DBus.Properties",
            "Get",
            g_variant_new("(ss)", "org.bluez.Device1", "Connected"),
            G_VARIANT_TYPE("(v)"),
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            nullptr,
            &error
        );
        
        if (error) {
            g_error_free(error);
            return false;
        }
        
        GVariant* variant;
        g_variant_get(result, "(v)", &variant);
        bool connected = g_variant_get_boolean(variant);
        
        g_variant_unref(variant);
        g_variant_unref(result);
        
        return connected;
    }
};

void printUsage() {
    std::cout << "\nUsage examples:" << std::endl;
    std::cout << "  1. Run discovery to find your headphones" << std::endl;
    std::cout << "  2. Note the MAC address (e.g., AA:BB:CC:DD:EE:FF)" << std::endl;
    std::cout << "  3. Pair and connect using that address" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "=== Bluetooth Audio Connection for Raspberry Pi ===" << std::endl;
    
    BluetoothManager bt;
    
    if (!bt.initialize()) {
        return 1;
    }
    
    if (!bt.powerOnAdapter()) {
        return 1;
    }
    
    // Start discovery
    if (!bt.startDiscovery()) {
        return 1;
    }
    
    // Wait for devices to be discovered
    std::cout << "\nScanning for 10 seconds..." << std::endl;
    sleep(10);
    
    bt.stopDiscovery();
    
    // List discovered devices
    auto devices = bt.getDiscoveredDevices();
    
    std::cout << "\n=== Discovered Devices ===" << std::endl;
    for (size_t i = 0; i < devices.size(); i++) {
        std::cout << i + 1 << ". " << devices[i].second 
                  << " [" << devices[i].first << "]" << std::endl;
    }
    
    if (devices.empty()) {
        std::cout << "No devices found. Make sure your headphones are in pairing mode." << std::endl;
        return 1;
    }
    
    // Example: Connect to first device (or specify MAC address)
    std::string target_mac;
    
    if (argc > 1) {
        // Use MAC address from command line argument
        target_mac = argv[1];
    } else {
        // Interactive selection
        std::cout << "\nEnter device number to connect (or 0 to exit): ";
        int choice;
        std::cin >> choice;
        
        if (choice <= 0 || choice > static_cast<int>(devices.size())) {
            std::cout << "Exiting..." << std::endl;
            return 0;
        }
        
        target_mac = devices[choice - 1].first;
    }
    
    std::cout << "\nConnecting to: " << target_mac << std::endl;
    
    // Pair, trust, and connect
    if (!bt.pairDevice(target_mac)) {
        std::cerr << "Pairing failed" << std::endl;
        return 1;
    }
    
    if (!bt.trustDevice(target_mac)) {
        std::cerr << "Trust failed" << std::endl;
        return 1;
    }
    
    if (!bt.connectDevice(target_mac)) {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }
    
    std::cout << "\n=== Successfully connected to Bluetooth headphones! ===" << std::endl;
    std::cout << "Audio should now route to your headphones." << std::endl;
    std::cout << "Press Enter to disconnect and exit..." << std::endl;
    
    std::cin.ignore();
    std::cin.get();
    
    bt.disconnectDevice(target_mac);
    
    return 0;
}