#pragma once

#include "../../../State.hpp"

class SearchingForDevices : public State {
public:
    explicit SearchingForDevices(EventBus& bus, Renderer& renderer, LoadingView& view, std::string _name)
    : State(bus, renderer, view, _name)
    {
        view.message = "Searching...";
    }

    void onEvent(const BackButtonPressed&) override {
        bus.publish(BluetoothDeviceSearchAbortRequested{});
        bus.publish(OpenConnectNewMenuRequested{});
    }
};