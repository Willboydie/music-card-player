#pragma once

#include "../GateState.hpp"

// TODO: Implement connecting logic
// TODO: this is a temporary state which reacts to the connection event. 
// I must create a new state class which is bound to a 
class BluetoothConnecting : public GateState {
public:
    Connecting(StateMachine& _context) : State(_context) {}
    
    void onEntry() override {
        std::cout << "Connecting..." << std::endl;
    }
    
    void onExit() override {}
    
    State* onBackButton() override;

private:
    void abortConnection();
};