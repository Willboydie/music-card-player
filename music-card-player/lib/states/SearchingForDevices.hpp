#pragma once

#include "LoadingState.hpp"

class SearchingForDevices : public LoadingState {
public:
    explicit SearchingForDevices(EventBus& bus, Renderer& renderer, LoadingView& view, std::string _name, Screen& _screen)
    : LoadingState(bus, renderer, view, _name, _screen)
    {
        loadingMessage = "Searching...";
    }

    void onEntry() override {
        LoadingState::onEntry();
        bus.publish(BluetoothDeviceSearchRequested{});
    }

    void onEvent(const BackButtonPressed&) override {
        bus.publish(BluetoothDeviceSearchAbortRequested{});
    }
};