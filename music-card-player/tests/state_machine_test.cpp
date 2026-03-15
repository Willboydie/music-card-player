#include "ui/event/EventBus.hpp"
#include "ui/screen/Screen.hpp"
#include "ui/view/Renderer.hpp"
#include "ui/state_machine/StateBuilder.hpp"
#include "ui/state_machine/state_id/StateId.hpp"
#include <iostream>
#include <cstdlib>

#define REQUIRE(cond) do { \
    if (!(cond)) { \
        std::cerr << "FAILED: " << #cond << "\n"; \
        std::exit(1); \
    } \
} while(0)

static void expectState(StateMachine& sm, StateId id) {
    REQUIRE(sm.getCurrentState() == sm.getState(id));
}

static void test_state_machine() {
    EventBus bus;
    Screen screen(2, 3);  // Don't call init() – avoids I2C
    Renderer renderer(screen);

    auto stateMachine = buildStateMachine(bus, renderer, screen);

    // Initial state
    expectState(*stateMachine, StateId::MAIN_MENU);

    // Forward transitions
    stateMachine->transitionTo(StateId::PLAYER);
    expectState(*stateMachine, StateId::PLAYER);

    stateMachine->transitionTo(StateId::BLUETOOTH_MENU);
    expectState(*stateMachine, StateId::BLUETOOTH_MENU);

    stateMachine->transitionTo(StateId::SAVED_DEVICES);
    expectState(*stateMachine, StateId::SAVED_DEVICES);

    stateMachine->transitionTo(StateId::CONNECT_NEW);
    expectState(*stateMachine, StateId::CONNECT_NEW);

    stateMachine->transitionTo(StateId::SEARCHING_FOR_DEVICES);
    expectState(*stateMachine, StateId::SEARCHING_FOR_DEVICES);

    stateMachine->transitionTo(StateId::FOUND_DEVICES);
    expectState(*stateMachine, StateId::FOUND_DEVICES);

    stateMachine->transitionTo(StateId::CONNECTING);
    expectState(*stateMachine, StateId::CONNECTING);

    // Back stack
    stateMachine->goBack();
    expectState(*stateMachine, StateId::FOUND_DEVICES);

    stateMachine->goBack();
    expectState(*stateMachine, StateId::SEARCHING_FOR_DEVICES);

    stateMachine->goBack();
    expectState(*stateMachine, StateId::CONNECT_NEW);

    stateMachine->goBack();
    expectState(*stateMachine, StateId::SAVED_DEVICES);

    stateMachine->goBack();
    expectState(*stateMachine, StateId::BLUETOOTH_MENU);

    stateMachine->goBack();
    expectState(*stateMachine, StateId::PLAYER);

    stateMachine->goBack();
    expectState(*stateMachine, StateId::MAIN_MENU);

    // Edge case: goBack when history empty – should be no-op
    stateMachine->goBack();
    expectState(*stateMachine, StateId::MAIN_MENU);

    // Edge case: transitionTo same state – no-op
    stateMachine->transitionTo(StateId::MAIN_MENU);
    expectState(*stateMachine, StateId::MAIN_MENU);

    std::cout << "State machine test passed\n";
}

int main() {
    test_state_machine();
    return 0;
}
