#pragma once

#include "MenuState.hpp"
#include "../storage/DeviceStorage.hpp"


class SavedDevicesMenu : public MenuState {
public:
    explicit SavedDevicesMenu(EventBus& bus, Renderer& renderer, MenuView& view, std::string _name, Screen& _screen)
    : MenuState(bus, renderer, view, _name, _screen)
    {
        menuTitle = "Saved Devices";
    }

    void onEntry() override {
        items.clear();
        loadSavedDevices();
        MenuState::onEntry();
    }

    using MenuState::onEvent;
    void onEvent(const BackButtonPressed&) override {
        bus.publish(OpenBluetoothMenuStateRequested{});
    }

private:
    void loadSavedDevices();
};