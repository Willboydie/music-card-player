#pragma once

#include "../event/EventBus.hpp"
#include "../event/Event.hpp"
#include "../state_machine/StateMachine.hpp"

// Subscribes to button-press and navigation events on the EventBus
// and dispatches them to the StateMachine.
//
// Button events are forwarded to the current state via onEvent().
// Navigation events trigger state transitions or goBack().

class NavigationHandler {
public:
    NavigationHandler(EventBus& bus, StateMachine& stateMachine);

private:
    EventBus& bus;
    StateMachine& sm;
};
