#pragma once

#include "State.hpp"
#include <memory>
#include <vector>

std::vector<std::unique_ptr<State>> buildStateMachine();

// States (version 1.0)

// Main Menu
//  - Player
//  - Bluetooth
// Music Player
//  - Playing
//  - Paused
//  - Volume Up
//  - Volume Down
// Bluetooth
//  - Disconnected
//     - Saved Devices
//        - Connect Device
//        - Delete Device
//     - Connect New (make sure device is in pairing mode)
//        - Searching
//        - Found Devices (list of devices)
//            - Connect to Device
//  - Connecting
//  - Connected

// Buttons
//  - Back
//  - Select
//  - Up
//  - Down