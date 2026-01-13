#pragma once

#include "State.hpp"
#include "state_id/StateId.hpp"
#include "screen_manager/ScreenManager.hpp"
#include "button_manager/ButtonManager.hpp"
#include <unordered_map>
#include <memory>

class StateMachine {
public:
    StateMachine(ScreenManager& _screenManager);
    ~StateMachine() = default;
    
    // State registration (called during initialization)
    void registerState(StateId id, std::unique_ptr<State> state);
    
    // State access (for states to get other states)
    State* getState(StateId id);
    
    // Transition management
    void transitionTo(StateId id);
    
    // Event handlers
    void onUpButton();
    void onDownButton();
    void onSelectButton();
    void onBackButton();

private:
    ScreenManager& screenManager;
    std::unordered_map<StateId, std::unique_ptr<State>> stateRegistry;
    State* currentState;
    StateId currentStateId;
    
    void performTransition(State* newState);
};