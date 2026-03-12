#pragma once

#include "../../../State.hpp"

class SearchingForDevices : public State {
public:
    explicit SearchingForDevices(EventBus& bus, Renderer& renderer, LoadingView& view, std::string _name)
    : State(bus, renderer, view, _name)
    {
        view.message = "Searching...";
    }

    void onEntry() override {
        State::onEntry();
        bus.publish(BluetoothDeviceSearchRequested{});
    }

    void onEvent(const BackButtonPressed&) override {
        bus.publish(BluetoothDeviceSearchAbortRequested{});
    }
};