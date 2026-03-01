#include "StateMachine.hpp"
#include "../../debugger/Debugger.hpp"

StateMachine::StateMachine()
    : currentState(nullptr)
    , currentStateId{}
{}

void StateMachine::setViews(std::unique_ptr<MenuView> mv,
                            std::unique_ptr<LoadingView> lv,
                            std::unique_ptr<PlayerView> pv) {
    menuView = std::move(mv);
    loadingView = std::move(lv);
    playerView = std::move(pv);
}


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
        // Push the current state onto the history stack before leaving
        // if (currentState) {
        //     history.push_back(currentStateId);
        // }
        performTransition(newState);
        currentStateId = id;
    }
}

// void StateMachine::goBack() {
//     if (history.empty()) return;

//     StateId previousId = history.back();
//     history.pop_back();

//     State* previousState = getState(previousId);
//     if (previousState) {
//         performTransition(previousState);
//         currentStateId = previousId;
//     }
// }

void StateMachine::performTransition(State* newState) {
    if (currentState) {
        currentState->onExit();
    }
    currentState = newState;
    if (currentState) {
        currentState->onEntry();
    }
}


// Note: onEvent is now a template defined in StateMachine.hpp.
// It forwards any event to the current state's onEvent handler.