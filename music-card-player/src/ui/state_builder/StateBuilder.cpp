#include "StateBuilder.hpp"
#include "screen_manager/ScreenManager.hpp"


std::unique_ptr<StateMachine> buildStateMachine(ScreenManager& screenManager) {
    auto stateMachine = std::make_unique<StateMachine>(screenManager);
    
    // Register all states with the state machine
    stateMachine->registerState(StateId::MAIN_MENU, 
        std::make_unique<MainMenuState>(*stateMachine));
    stateMachine->registerState(StateId::PLAYER, 
        std::make_unique<PlayerState>(*stateMachine));
    stateMachine->registerState(StateId::BLUETOOTH_MENU, 
        std::make_unique<BluetoothMenuState>(*stateMachine));
    stateMachine->registerState(StateId::SAVED_DEVICES, 
        std::make_unique<SavedDevices>(*stateMachine));
    stateMachine->registerState(StateId::CONNECT_NEW, 
        std::make_unique<ConnectNew>(*stateMachine));
    stateMachine->registerState(StateId::SEARCHING_FOR_DEVICES, 
        std::make_unique<SearchingForDevices>(*stateMachine));
    stateMachine->registerState(StateId::FOUND_DEVICES, 
        std::make_unique<FoundDevices>(*stateMachine));
    stateMachine->registerState(StateId::CONNECTING, 
        std::make_unique<Connecting>(*stateMachine));
    
    // Set initial state
    stateMachine->transitionTo(StateId::MAIN_MENU);
    
    return stateMachine;
}