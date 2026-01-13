#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <functional>
#include <cstdlib>

#include "state_id/StateId.hpp"

// Forward declaration
class State;

class MenuOption {
public:
    StateId stateId;
    std::function<State*()> onSelect;

    MenuOption(StateId _stateId, std::function<State*()> _onSelect) 
    : stateId(_stateId)
    , onSelect(_onSelect) 
    {}

};


class Menu {
public:

    Menu() : selected_index(0), options() {}

    // Add a menu option at runtime
    void addOption(StateId stateId, std::function<State*()> onSelect);

    // Navigation methods
    void moveUp();
    void moveDown();
    void select();

    // Getters
    size_t getSelectedIndex() const;
    StateId getSelectedStateId() const;
    const std::vector<MenuOption>& getOptions() const;
    size_t getOptionCount() const;
    
    // Clear all options
    void clear();

private:
    std::vector<MenuOption> options;
    size_t selected_index;
};