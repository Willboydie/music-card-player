#pragma once

#include <cstddef>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include "../debugger/Debugger.hpp"

// Type-safe publish / subscribe event bus.
//
// Any struct can serve as an event — no base class needed.
//
//   EventBus bus;
//
//   auto id = bus.subscribe<VolumeChangeRequested>([](const VolumeChangeRequested& e) {
//       setVolume(e.level);
//   });
//
//   bus.publish(VolumeChangeRequested{ 0.8f });
//
//   bus.unsubscribe(id);

class EventBus {
public:
    using SubscriptionId = std::size_t;

    EventBus() = default;
    ~EventBus() = default;

    // Register a callback for events of type T.
    // Returns an id that can be passed to unsubscribe().
    template<typename T>
    SubscriptionId subscribe(std::function<void(const T&)> callback) {
        SubscriptionId id = nextId++;
        auto key = std::type_index(typeid(T));

        subscriptions[key].push_back({
            id,
            [cb = std::move(callback)](const void* raw) {
                cb(*static_cast<const T*>(raw));
            }
        });

        return id;
    }

    // Deliver an event to every subscriber of that event type.
    template<typename T>
    void publish(const T& event) {
        Debugger::debug_msg("EventBus: publishing " + std::string(typeid(T).name()));
        auto it = subscriptions.find(std::type_index(typeid(T)));
        if (it == subscriptions.end()) return;

        // Iterate over a copy of the handler list so that a handler
        // may safely subscribe or unsubscribe during dispatch.
        auto handlers = it->second;
        for (auto& entry : handlers) {
            entry.handler(&event);
        }
    }

    // Remove a previously registered subscription.
    void unsubscribe(SubscriptionId id) {
        for (auto& [key, entries] : subscriptions) {
            entries.erase(
                std::remove_if(entries.begin(), entries.end(),
                    [id](const Entry& e) { return e.id == id; }),
                entries.end());
        }
    }

private:
    using Handler = std::function<void(const void*)>;

    struct Entry {
        SubscriptionId id;
        Handler handler;
    };

    std::unordered_map<std::type_index, std::vector<Entry>> subscriptions;
    SubscriptionId nextId = 0;
};
