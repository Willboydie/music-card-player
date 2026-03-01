#pragma once

#include <string>

// When defining and naming an event think: has this definitely happened when its designed to be emitted?
// Or is it a request for something to happen?
// all possible events

// Physical UI events
// buttons
struct UpButtonPressed {};
struct DownButtonPressed {};
struct SelectButtonPressed {};
struct BackButtonPressed {};

// User Intent Events
// navigation
struct BackNavigationRequested {};
struct OpenPlayerStateRequested {};
struct OpenBluetoothMenuStateRequested {};
struct OpenSavedDevicesMenuRequested {};
struct OpenConnectNewMenuRequested {};
struct OpenSearchingForDevicesMenuRequested {};
struct OpenBluetoothConnectingMenuRequested {};
struct OpenFoundDevicesMenuRequested {};

// player
struct MusicPlayRequested {};
struct MusicPauseRequested {};
struct VolumeChangeRequested {
    float level;
};

// bluetooth
struct BluetoothConnectionRequested {
    std::string device_info;
};
struct BluetoothConnected {};
struct BluetoothConnectionAbortRequested {};
struct BluetoothConnectionFailed {};
struct BluetoothDeviceSearchRequested {
    std::string search_info;
};
struct BluetoothDeviceSearchAbortRequested {};
struct BluetoothDeviceSearchComplete {};
struct BluetoothDeviceDiscovered {
    std::string info;
};