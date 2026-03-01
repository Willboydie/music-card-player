#pragma once

#include "../roles/menu_state/MenuState.hpp"
#include "../../event/Event.hpp"


class BluetoothMenuState : public MenuState {
public:
    explicit BluetoothMenuState(EventBus& bus, Renderer& renderer, MenuView& view, std::string _name)
    : MenuState(bus, renderer, view, _name)
    {
        view.title = "Bluetooth";
        items = {
            { MenuOption::BluetoothSettings, [](EventBus& b){ b.publish(OpenSavedDevicesMenuRequested{}); }, "Saved Devices" },
            { MenuOption::DynamicStartBluetooth, [](EventBus& b){ b.publish(OpenConnectNewMenuRequested{}); }, "Connect New" }
        };
    }

    using MenuState::onEvent;
    void onEvent(const BackButtonPressed&) override {
        bus.publish(OpenMainMenuStateRequested{});
    }
};