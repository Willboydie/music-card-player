#pragma once

#include "State.hpp"
#include "state/MenuState.hpp"
#include "menu/Menu.hpp"
#include "state_id/StateId.hpp"
#include "state_machine/StateMachine.hpp"
#include "storage/DeviceStorage.hpp"
#include <memory>
#include <iostream>


// Function to build and initialize the state machine
std::unique_ptr<StateMachine> buildStateMachine(ScreenManager& screenManager);