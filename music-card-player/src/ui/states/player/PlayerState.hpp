#pragma once

#include "../State.hpp"


class PlayerState : public State {
    public:
        PlayerState(StateMachine& _context) : State(_context) {}
    
        void onEntry() override {
            std::cout << "Player Active" << std::endl;
        }
        
        void onExit() override {}
        
        State* onUpButton() override {
            // volume up event
            return this;
        }
        
        State* onDownButton() override {
            // volume down event
            return this;
        }
        
        State* onSelectButton() override {
            // play/pause event
            return this;
        }
        
        State* onBackButton() override {
            // Go back to main menu
            return getState(StateId::MAIN_MENU);
        }
    };