#pragma once

#include "Screen.hpp"
#include "Menu.hpp"
#include "state_id/StateId.hpp"

class StateMachine;

class State {
protected:
    StateMachine& context;
public:
    State(StateMachine& _context) : context(_context) {}
    virtual ~State() = default;

    virtual void onEntry() = 0;
    virtual void onExit() = 0;

    // These button event handlers return a pointer to the state to transition to
    virtual State* onUpButton() { return this };
    virtual State* onDownButton() { return this };
    virtual State* onSelectButton() { return this };
    virtual State* onBackButton() { return this };

    // Helper method to get state by ID through context
    State* getState(StateId stateId);
};
