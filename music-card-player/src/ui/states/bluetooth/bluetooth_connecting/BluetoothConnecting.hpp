#pragma once

#include "../../State.hpp"

class BluetoothConnecting : public State {
public:
    explicit BluetoothConnecting(EventBus& bus, Renderer& renderer, LoadingView& view, std::string _name)
    : State(bus, renderer, view, _name)
    {
        view.message = "Connecting...";
    }

    void onEvent(const BackButtonPressed&) override {
        bus.publish(BluetoothConnectionAbortRequested{});
    }
};