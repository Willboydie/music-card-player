#pragma once
#include "../utils/constants.h"
#include "../event/EventBus.hpp"
#include "../ui/button/ButtonListener.hpp"
#include "../ui/screen/Screen.hpp"
#include "../ui/view/Renderer.hpp"
#include "../ui/state_machine/StateMachine.hpp"
#include "../ui/state_machine/StateBuilder.hpp"
#include "../audio/AudioManager.hpp"
#include "../bluetooth/BluetoothManager.hpp"
#include "../handlers/NavigationHandler.hpp"
#include "../handlers/AudioHandler.hpp"
#include "../handlers/BluetoothHandler.hpp"
#include "../debugger/Debugger.hpp"

#include <iostream>
#include <chrono>
#include <thread>

// Top-level application controller.
//
// Owns and wires together every subsystem:
//   EventBus  ←→  ButtonListener, Screen, StateMachine,
//                  AudioManager, BluetoothManager,
//                  NavigationHandler, AudioHandler, BluetoothHandler

class AppController {
public:
    AppController() = default;
    ~AppController() = default;

    void run();
};