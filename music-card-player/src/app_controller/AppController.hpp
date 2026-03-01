#pragma once
#include "../utils/constants.h"

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