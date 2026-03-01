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
        State::onEntry();
    }

    // ----- input interpretation -----
    void onEvent(const UpButtonPressed&) override { moveSelection(-1); }
    void onEvent(const DownButtonPressed&) override { moveSelection(1); }
    void onEvent(const SelectButtonPressed&) override { onSelectCurrentItem(); }
    void onEvent(const BackButtonPressed&) override { bus.publish(BackNavigationRequested{}); }

protected:
    MenuView& view;
    std::vector<MenuItem> items;
    int selectedIndex = 0;

    void onSelectCurrentItem() {
        if (items.empty()) return;
        const MenuItem& item = items[selectedIndex];
        if (!item.enabled) return;
        if (item.onSelect) item.onSelect(bus);
    }

    // Copies items → view.items and syncs selectedIndex
    void syncView();
    void moveSelection(int delta);
    void ensureValidSelection();
};