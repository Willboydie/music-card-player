#pragma once

#include "../ui/event/EventBus.hpp"
#include "../ui/event/Event.hpp"
#include "../audio/AudioManager.hpp"

// Subscribes to audio-related events on the EventBus and
// dispatches them to the AudioManager.

class AudioHandler {
public:
    AudioHandler(EventBus& bus, AudioManager& audioManager);

private:
    EventBus& bus;
    AudioManager& audio;
};
