#pragma once

#include <cstdint>
#include <functional>

enum class StateId : uint8_t {
    MAIN_MENU,
    PLAYER,
    BLUETOOTH_MENU,
    SAVED_DEVICES,
    CONNECT_NEW,
    SEARCHING_FOR_DEVICES,
    FOUND_DEVICES,
    CONNECTING
};

// Allow StateId to be used as an unordered_map key
namespace std {
    template<>
    struct hash<StateId> {
        std::size_t operator()(StateId id) const noexcept {
            return std::hash<uint8_t>{}(static_cast<uint8_t>(id));
        }
    };
}
