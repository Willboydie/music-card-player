#pragma once

#include "../states/State.hpp"
#include "../view/View.hpp"
#include "state_id/StateId.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>
#include "../../debugger/Debugger.hpp"

class StateMachine {
public:
    StateMachine();
    ~StateMachine() = default;

    // View ownership (set before registerState; views must outlive states)
    void setViews(std::unique_ptr<MenuView> menuView,
                  std::unique_ptr<LoadingView> loadingView,
                  std::unique_ptr<PlayerView> playerView);

    MenuView* getMenuView() { return menuView.get(); }
    LoadingView* getLoadingView() { return loadingView.get(); }
    PlayerView* getPlayerView() { return playerView.get(); }

    // State registration (called during initialisation)
    void registerState(StateId id, std::unique_ptr<State> state);

    // State access
    State* getState(StateId id);
    State* getCurrentState() { return currentState; }

    // Transition management
    void transitionTo(StateId id);
    // void goBack();

    // Forward any event to the current state's onEvent handler
    template<typename Event>
    void onEvent(const Event& event) {
        if (currentState) {
            currentState->onEvent(event);
        }
    }

private:
    std::unique_ptr<MenuView> menuView;
    std::unique_ptr<LoadingView> loadingView;
    std::unique_ptr<PlayerView> playerView;
    std::unordered_map<StateId, std::unique_ptr<State>> stateRegistry;
    State* currentState;
    StateId currentStateId;
    // std::vector<StateId> history;

    void performTransition(State* newState);
};