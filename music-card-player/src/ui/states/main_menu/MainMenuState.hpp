#pragma once

#include "../roles/menu_state/MenuState.hpp"


class MainMenuState : public MenuState {
public:
    explicit MainMenuState(EventBus& bus, Renderer& renderer, MenuView& view, std::string _name)
    : MenuState(bus, renderer, view, _name)
    {
        view.title = "Main Menu";
        items = {
            { [](EventBus& b){ b.publish(OpenPlayerStateRequested{}); }, "Player" },
            { [](EventBus& b){ b.publish(OpenBluetoothMenuStateRequested{}); }, "Bluetooth" }
        };
    }

    // Main menu has no back
    using MenuState::onEvent;
    void onEvent(const BackButtonPressed&) override { }
};