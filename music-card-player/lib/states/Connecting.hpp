#pragma once

#include "LoadingState.hpp"

class Connecting : public LoadingState {
public:
    explicit Connecting(EventBus& bus, Renderer& renderer, LoadingView& view, std::string _name, Screen& _screen)
    : LoadingState(bus, renderer, view, _name, _screen)
    {
        loadingMessage = "Connecting...";
    }

    void onEntry() override {
        LoadingState::onEntry();
        bus.publish(BluetoothConnectionRequested{ info });
    }

    void onEvent(const BackButtonPressed&) override {
        bus.publish(BluetoothConnectionAbortRequested{});
    }
};