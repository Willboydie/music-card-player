#include "State.hpp"
#include "state_machine/StateMachine.hpp"

State* State::getState(StateId stateId) {
    return context.getState(stateId);
}
