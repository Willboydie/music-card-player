#pragma once

#include "MenuState.hpp"
#include "../storage/DeviceStorage.hpp"

class FoundDevicesMenu : public MenuState {
public:
    explicit FoundDevicesMenu(EventBus& bus, Renderer& renderer, MenuView& view, std::string _name, Screen& _screen)
    : MenuState(bus, renderer, view, _name, _screen)
    {
        menuTitle = "Found Devices";
        loadFoundDevices();
    }

    void onEntry() override {
        items.clear();
        loadFoundDevices();
        MenuState::onEntry();
    }
    using MenuState::onEvent;
    void onEvent(const BackButtonPressed&) override {
        bus.publish(OpenConnectNewMenuRequested{});
    }


private:
    void loadFoundDevices();
};