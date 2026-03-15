#pragma once

#include "MenuState.hpp"


class BluetoothMenuState : public MenuState {
public:
    explicit BluetoothMenuState(EventBus& bus, Renderer& renderer, MenuView& view, std::string _name, Screen& _screen)
    : MenuState(bus, renderer, view, _name, _screen)
    {
        menuTitle = "Bluetooth";
        items = {
            { [](EventBus& b){ b.publish(OpenSavedDevicesMenuRequested{}); }, "Saved Devices" },
            { [](EventBus& b){ b.publish(OpenConnectNewMenuRequested{}); }, "Connect New" }
        };
    }

    using MenuState::onEvent;
    void onEvent(const BackButtonPressed&) override {
        bus.publish(OpenMainMenuStateRequested{});
    }
};