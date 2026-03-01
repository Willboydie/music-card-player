#pragma once

#include "../../../roles/menu_state/MenuState.hpp"
#include "../../../../../storage/DeviceStorage.hpp"

class FoundDevicesMenu : public MenuState {
public:
    explicit FoundDevicesMenu(EventBus& bus, Renderer& renderer, MenuView& view, std::string _name)
    : MenuState(bus, renderer, view, _name)
    {
        view.title = "Found Devices";
        loadFoundDevices();
    }

    void onEntry() override {
        items.clear();
        loadFoundDevices();
        MenuState::onEntry();
    }


private:
    void loadFoundDevices();
};