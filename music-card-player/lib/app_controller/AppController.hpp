#pragma once
#include "../utils/constants.h"
#include "../event/EventBus.hpp"
#include "../button/ButtonListener.hpp"
#include "../screen/Screen.hpp"
#include "../view/Renderer.hpp"
#include "../state_machine/StateMachine.hpp"
#include "../state_machine/StateBuilder.hpp"
#include "../audio/AudioManager.hpp"
#include "../bluetooth/BluetoothManager.hpp"
#include "../handlers/NavigationHandler.hpp"
#include "../handlers/AudioHandler.hpp"
#include "../handlers/BluetoothHandler.hpp"
#include "../debugger/Debugger.hpp"
#include "../storage/DeviceStorage.hpp"

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

    void run(bool debug);
};