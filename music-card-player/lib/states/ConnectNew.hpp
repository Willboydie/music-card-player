#pragma once

#include "MenuState.hpp"

class ConnectNew : public MenuState {
public:
    explicit ConnectNew(EventBus& bus, Renderer& renderer, MenuView& view, std::string name, Screen& _screen)
    : MenuState(bus, renderer, view, name, _screen)
    {
        menuTitle = "Connect New";
        items = {
            { [](EventBus& b){
                b.publish(OpenSearchingForDevicesMenuRequested{});
            }, "Search for Devices" }
        };
    }

    using MenuState::onEvent;
    void onEvent(const BackButtonPressed&) override {
        bus.publish(OpenBluetoothMenuStateRequested{});
    }
};