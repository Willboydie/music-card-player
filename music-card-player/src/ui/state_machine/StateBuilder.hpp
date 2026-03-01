#pragma once

#include "StateMachine.hpp"
#include "state_id/StateId.hpp"
#include "../states/main_menu/MainMenuState.hpp"
#include "../states/player/PlayerState.hpp"
#include "../states/bluetooth/BluetoothMenuState.hpp"
#include "../states/bluetooth/saved_devices/SavedDevicesMenu.hpp"
#include "../states/bluetooth/connect_new/ConnectNew.hpp"
#include "../states/bluetooth/connect_new/searching_for_devices/SearchingForDevices.hpp"
#include "../states/bluetooth/connect_new/found_devices/FoundDevicesMenu.hpp"
#include "../states/bluetooth/bluetooth_connecting/BluetoothConnecting.hpp"
#include <memory>

// Forward declarations
class EventBus;
class Renderer;

// Build and initialise the state machine with all registered states.
std::unique_ptr<StateMachine> buildStateMachine(EventBus& bus, Renderer& renderer);