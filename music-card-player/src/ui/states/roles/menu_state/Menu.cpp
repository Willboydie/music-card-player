#pragma once

#include "Menu.hpp"

// Add a menu option at runtime
void Menu::addOption(StateId stateId, std::function<State*()> onSelect) {
    options.emplace_back(stateId, onSelect);
}

// Navigation methods
void Menu::moveUp() {
    if (!options.empty()) {
        selected_index = (selected_index == 0) ? options.size() - 1 : selected_index - 1;
    }
}

void Menu::moveDown() {
    if (!options.empty()) {
        selected_index = (selected_index + 1) % options.size();
    }
}

State* Menu::select() {
    if (!options.empty() && selected_index < options.size()) {
        if (options[selected_index].onSelect) {
            return options[selected_index].onSelect();
        }
    }
    return nullptr;
}

// Getters
const std::string& Menu::getSelectedLabel() const {
    static const std::string empty;
    if (!options.empty() && selected_index < options.size()) {
        return options[selected_index].label;
    }
    return empty;
}

// Clear all options
void Menu::clear() {
    options.clear();
    selected_index = 0;
}