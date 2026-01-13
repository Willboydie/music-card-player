#include "StateMachine.hpp"

StateMachine::StateMachine(ScreenManager& _screenManager)
    : screenManager(_screenManager), currentState(nullptr) {}

void StateMachine::registerState(StateId id, std::unique_ptr<State> state) {
    stateRegistry[id] = std::move(state);
}

State* StateMachine::getState(StateId id) {
    auto it = stateRegistry.find(id);
    return (it != stateRegistry.end()) ? it->second.get() : nullptr;
}

void StateMachine::transitionTo(StateId id) {
    State* newState = getState(id);
    if (newState && newState != currentState) {
        performTransition(newState);
        currentStateId = id;
    }
}

void StateMachine::performTransition(State* newState) {
    if (currentState) {
        currentState->onExit();
    }
    currentState = newState;
    currentStateId = newState->getStateId();
    if (currentState) {
        currentState->onEntry();
    }
    // screenManager.set(newScreen);   // TODO: Implement screen management

}

void StateMachine::onUpButton() {
    if (currentState) {
        State* nextState = currentState->onUpButton();
        if (nextState && nextState != currentState) {
            performTransition(nextState);
        }
    }
}

void StateMachine::onDownButton() {
    if (currentState) {
        State* nextState = currentState->onDownButton();
        if (nextState && nextState != currentState) {
            performTransition(nextState);
        }
    }
}

void StateMachine::onSelectButton() {
    if (currentState) {
        State* nextState = currentState->onSelectButton();
        if (nextState && nextState != currentState) {
            performTransition(nextState);
        }
    }
}

void StateMachine::onBackButton() {
    if (currentState) {
        State* nextState = currentState->onBackButton();
        if (nextState && nextState != currentState) {
            performTransition(nextState);
        }
    }
}