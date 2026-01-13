#pragma once

#include "State.hpp"
#include "menu/Menu.hpp"

class MenuState : public State {
public:
    MenuState(StateMachine& _context) 
        : State(_context), menu() {}

    State* onUpButton() override {
        menu.moveUp();
        return this;
    }
    
    State* onDownButton() override {
        menu.moveDown();
        return this;
    }
    
    State* onSelectButton() override {
        State* result = menu.select();
        if (result) {
            return result;
        }
        return this;
    }

    virtual State* onBackButton() override = 0; 

protected:
    Menu menu;
};