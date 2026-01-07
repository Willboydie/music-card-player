#pragma once

#include "Screen.hpp"

class State {
public:
    State(Screen& screen) : screen(screen) {}
    virtual ~State() = default;

    Screen& screen;
    State* up = nullptr;
    State* down = nullptr;
    State* select = nullptr;
    State* back = nullptr;

    virtual void onEntry() = 0;
    virtual void onExit() = 0;
};
