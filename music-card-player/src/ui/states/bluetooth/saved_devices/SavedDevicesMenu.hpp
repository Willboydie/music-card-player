#pragma once

#include "state/MenuState.hpp"
#include "state_id/StateId.hpp"
#include "state_machine/StateMachine.hpp"
#include "storage/DeviceStorage.hpp"
#include <memory>
#include <iostream>


class SavedDevices : public MenuState {
    public:
        
        void onEntry() override {
            // Reload devices in case they changed
            menu.clear();
            // Load saved devices from storage and populate menu
            loadSavedDevices();
            std::cout << "Saved Devices" << std::endl;
        }
        
        void onExit() override {}
        
        State* onBackButton() override {
            // Go back to Bluetooth menu
            return getState(StateId::BLUETOOTH_MENU);
        }
    
    private:
        void loadSavedDevices();
    };