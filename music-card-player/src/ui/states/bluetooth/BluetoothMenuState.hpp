#pragma once
#include "../MenuState.hpp"
#include "../../state_id/StateId.hpp"
#include "../../state_machine/StateMachine.hpp"
#include <iostream>


class BluetoothMenuState : public MenuState {
    public:
        BluetoothMenuState(StateMachine& _context) : MenuState(_context) {
            menu.addOption(StateId::SAVED_DEVICES, [this]() {
                return getState(StateId::SAVED_DEVICES);
            });
            menu.addOption(StateId::CONNECT_NEW, [this]() {
                return getState(StateId::CONNECT_NEW);
            });
        }
        
        void onEntry() override { std::cout << "Bluetooth Menu" << std::endl; }
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
            // Go back to main menu
            return getState(StateId::BLUETOOTH_MENU);
        }
    };