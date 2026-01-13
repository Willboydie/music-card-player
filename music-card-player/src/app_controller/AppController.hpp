#pragma once

#include "state_machine/StateMachine.hpp"
#include "state_builder/StateBuilder.hpp"
#include "button_manager/ButtonManager.hpp"
#include "screen_manager/ScreenManager.hpp"

class AppController {
public:
    AppController() {}
    ~AppController() = default;
    void run();
};