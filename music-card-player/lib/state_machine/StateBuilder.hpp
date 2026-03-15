#pragma once

#include "StateMachine.hpp"
#include "StateId.hpp"
#include "../states/MainMenuState.hpp"
#include "../states/PlayerState.hpp"
#include "../states/BluetoothMenuState.hpp"
#include "../states/SavedDevicesMenu.hpp"
#include "../states/ConnectNew.hpp"
#include "../states/SearchingForDevices.hpp"
#include "../states/FoundDevicesMenu.hpp"
#include "../states/Connecting.hpp"
#include "../event/EventBus.hpp"
#include "../view/Renderer.hpp"
#include "../view/View.hpp"
#include "../screen/Screen.hpp"

#include <memory>

// Forward declarations
class EventBus;
class Renderer;
// Build and initialise the state machine with all registered states.
std::unique_ptr<StateMachine> buildStateMachine(EventBus& bus, Renderer& renderer, Screen& screen);