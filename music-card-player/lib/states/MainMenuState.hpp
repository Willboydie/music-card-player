#pragma once

#include "MenuState.hpp"

class MainMenuState : public MenuState {
public:
    explicit MainMenuState(EventBus& bus, Renderer& renderer, MenuView& view, std::string _name, Screen& _screen)
    : MenuState(bus, renderer, view, _name, _screen)
    {
        menuTitle = "Main Menu";
        items = {
            { [](EventBus& b){ b.publish(OpenPlayerStateRequested{}); }, "Player" },
            { [](EventBus& b){ b.publish(OpenBluetoothMenuStateRequested{}); }, "Bluetooth" }
        };
    }

    void onEvent(const BackButtonPressed&) override {
        bus.publish(ExitRequested{});
    }
};