#pragma once

#include "../State.hpp"

class ConnectNew : public State {
public:
    ConnectNew(StateMachine& _context) : State(_context) {}
    
    void onEntry() override {
        std::cout << "Click to search for devices. Make sure your device is in pairing mode." << std::endl;
    }
    
    void onExit() override {}
    
    State* onSelectButton() override {
        // Start searching for devices
        return getState(StateId::SEARCHING_FOR_DEVICES);
    }
    
    State* onBackButton() override {
        // Go back to Bluetooth menu
        return getState(StateId::BLUETOOTH_MENU);
    }
};