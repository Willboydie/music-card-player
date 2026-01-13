#include "SavedDevicesMenu.hpp"

void SavedDevices::loadSavedDevices() {
    // Load devices from storage
    auto devices = DeviceStorage::loadDevices();
    
    // Add each device to the menu
    for (const auto& device : devices) {
        // Store device address for later use
        std::string deviceAddress = device.address;
        
        // Add menu option that transitions to connecting state
        // The connecting state will use the device address
        menu.addOption(StateId::CONNECTING, [this, deviceAddress]() {
            // TODO: Set the target device address for connection
            // This could be stored in StateMachine or a shared context
            // For now, transition to connecting state
            return getState(StateId::CONNECTING);
        });
    }
}
// check this over (AI generated)