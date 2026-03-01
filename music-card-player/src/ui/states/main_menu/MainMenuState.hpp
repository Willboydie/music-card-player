#pragma once

#include "../roles/menu_state/MenuState.hpp"
#include "../../event/Event.hpp"


class MainMenuState : public MenuState {
public:
    explicit MainMenuState(EventBus& bus, Renderer& renderer, MenuView& view, std::string _name)
    : MenuState(bus, renderer, view, _name)
    {
        view.title = "Main Menu";
        items = {
            { MenuOption::AudioSettings, [](EventBus& b){ b.publish(OpenPlayerStateRequested{}); }, "Player" },
            { MenuOption::BluetoothSettings, [](EventBus& b){ b.publish(OpenBluetoothMenuStateRequested{}); }, "Bluetooth" }
        };
    }

    // Main menu has no back
    void onEvent(const BackButtonPressed&) override { }
};