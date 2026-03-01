#pragma once

#include "../../State.hpp"
#include "MenuOption.hpp"
#include "MenuItem.hpp"
#include <vector>


class MenuState : public State {
public:
    explicit MenuState(EventBus& bus, Renderer& renderer, MenuView& _view, std::string _name)
    : State(bus, renderer, _view, _name)
    , view(_view)
    {}

    // ----- lifecycle -----
    void onEntry() override {
        selectedIndex = 0;
        syncView();
        ensureValidSelection();
        Debugger::debug_msg("MenuState: Selected Option: " + items[selectedIndex].label);
        State::onEntry();
    }

    // ----- input interpretation -----
    void onEvent(const UpButtonPressed&) override { moveSelection(-1); }
    void onEvent(const DownButtonPressed&) override { moveSelection(1); }
    void onEvent(const SelectButtonPressed&) override { onSelectCurrentItem(); }

protected:
    MenuView& view;
    std::vector<MenuItem> items;
    int selectedIndex = 0;

    void onSelectCurrentItem() {
        if (items.empty()) return;
        const MenuItem& item = items[selectedIndex];
        if (!item.enabled) return;
        if (item.onSelect) item.onSelect(bus);
        Debugger::debug_msg("MenuState: clicked option: " + item.label);
    }

    // Copies items → view.items and syncs selectedIndex
    void syncView();
    void moveSelection(int delta);
    void ensureValidSelection();

};