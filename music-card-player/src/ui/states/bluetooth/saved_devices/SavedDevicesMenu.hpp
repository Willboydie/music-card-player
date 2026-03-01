#pragma once

#include "../../roles/menu_state/MenuState.hpp"
#include "../../../../storage/DeviceStorage.hpp"


class SavedDevicesMenu : public MenuState {
public:
    explicit SavedDevicesMenu(EventBus& bus, Renderer& renderer, MenuView& view, std::string _name)
    : MenuState(bus, renderer, view, _name)
    {
        view.title = "Saved Devices";
    }

    void onEntry() override {
        items.clear();
        loadSavedDevices();
        MenuState::onEntry();
    }

private:
    void loadSavedDevices();
};