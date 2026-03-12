#pragma once

#include "../../roles/menu_state/MenuState.hpp"

class ConnectNew : public MenuState {
public:
    explicit ConnectNew(EventBus& bus, Renderer& renderer, MenuView& view, std::string name)
    : MenuState(bus, renderer, view, name)
    {
        view.title = "Connect New";
        items = {
            { [](EventBus& b){
                b.publish(BluetoothDeviceSearchRequested{});
            }, "Search for Devices" }
        };
    }

    using MenuState::onEvent;
    void onEvent(const BackButtonPressed&) override {
        bus.publish(OpenBluetoothMenuStateRequested{});
    }
};