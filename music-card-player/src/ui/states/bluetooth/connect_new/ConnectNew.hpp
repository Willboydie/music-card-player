#pragma once

#include "../../State.hpp"

class ConnectNew : public State {
public:
    explicit ConnectNew(EventBus& bus, Renderer& renderer, LoadingView& view, std::string name)
    : State(bus, renderer, view, name)
    {
        view.message = "Press select to search";
    }

    void onEvent(const SelectButtonPressed&) override {
        bus.publish(BluetoothDeviceSearchRequested{});
    }

    void onEvent(const BackButtonPressed&) override {
        bus.publish(BackNavigationRequested{});
    }
};