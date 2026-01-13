#pragma once
#include "../MenuState.hpp"
#include "../../state_id/StateId.hpp"
#include "../../state_machine/StateMachine.hpp"
#include <iostream>


class MainMenuState : public MenuState {
    public:
        MainMenuState(StateMachine& _context) : MenuState(_context) {
            menu.addOption(StateId::PLAYER, [this]() {
                return getState(StateId::PLAYER);
            });
            menu.addOption(StateId::BLUETOOTH_MENU, [this]() {
                return getState(StateId::BLUETOOTH_MENU);
            });
        }
        
        void onEntry() override { std::cout << "Main Menu" << std::endl; }
        void onExit() override {}
        
        State* onUpButton() override {
            menu.moveUp();
            return this;  // Stay in current state
        }
        
        State* onDownButton() override {
            menu.moveDown();
            return this;  // Stay in current state
        }
        
        State* onSelectButton() override {
            State* result = menu.select();
            if (result) {
                return result;
            }
            return this;
        }
        
        State* onBackButton() override {
            return this;  // Main menu has no back
        }
    };