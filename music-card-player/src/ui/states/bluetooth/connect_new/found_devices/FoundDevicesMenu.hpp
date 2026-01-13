#pragma once

#include "state/MenuState.hpp"
#include "state_id/StateId.hpp"
#include "state_machine/StateMachine.hpp"
#include "storage/DeviceStorage.hpp"
#include <memory>
#include <iostream>


class FoundDevicesMenu : public State {
    public:
        FoundDevices(StateMachine& _context) : MenuState(_context) {
            // Load found devices from storage and populate menu
            loadFoundDevices();
        }
        
        void onEntry() override {
            // Reload devices in case they changed
            menu.clear();
            loadFoundDevices();
            std::cout << "Found Devices. Select a device to connect to." << std::endl;
        }
        
        void onExit() override {}
        
        State* onUpButton() override {
            return this;
        }
        
        State* onDownButton() override {
            return this;
        }
        
        State* onSelectButton() override {
            // Start connecting
            return getState(StateId::CONNECTING);
        }
        
        State* onBackButton() override {
            // Go back to searching
            return getState(StateId::SEARCHING_FOR_DEVICES);
        }
    
    private:
        void loadFoundDevices();
    };