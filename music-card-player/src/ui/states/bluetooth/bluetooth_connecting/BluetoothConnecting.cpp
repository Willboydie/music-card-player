#include "BluetoothConnecting.hpp"


State* BluetoothConnecting::onBackButton() {
    abortConnection();
    cancelled = true;
    return this;
}

void BluetoothConnecting::abortConnection() {
    // to implement
    // something like:
    // void abortBluetoothConnection(std::function<void(bool)> onDone) {
    //     bluetoothThread.enqueue([onDone = std::move(onDone)] {
    //         bool ok = shutdownBluetooth();
    //         mainThread.enqueue([ok, onDone] {
    //             onDone(ok);
    //         });
    //     });
    // }
}