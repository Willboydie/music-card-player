#pragma once

enum class MenuOption : int {
    // Static menu options
    AudioSettings     = 0,
    BluetoothSettings = 1,
    DisplaySettings   = 2,
    ShuffleToggle     = 3,
    Volume            = 4,
    ResetDevice       = 5,

    // Reserve ranges for dynamic menus
    DynamicStartBluetooth = 1000,
    DynamicStartPlaylist   = 2000,
    DynamicStartDevices    = 3000
};