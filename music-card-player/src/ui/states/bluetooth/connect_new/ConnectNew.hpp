#pragma once

#include "../../State.hpp"
#include "../../../event/Event.hpp"

class ConnectNew : public State {
public:
    explicit ConnectNew(EventBus& bus, Renderer& renderer, LoadingView& view, std::string name)
    : State(bus, renderer, view, name)
    {
        view.message = "Press select to search";
    }

    void onEvent(const SelectButtonPressed&) override {
        bus.publish(BluetoothDeviceSearchRequested{});
        bus.publish(OpenSearchingForDevicesMenuRequested{});
    }

    void onEvent(const BackButtonPressed&) override {
        bus.publish(BluetoothConnectionAbortRequested{});
        bus.publish(OpenBluetoothMenuStateRequested{});
    }
};