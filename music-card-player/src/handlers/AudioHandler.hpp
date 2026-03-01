#pragma once

#include "../ui/event/EventBus.hpp"
#include "../ui/event/Event.hpp"
#include "../audio/IAudioManager.hpp"

class AudioHandler {
public:
    AudioHandler(EventBus& bus, IAudioManager& audioManager);

private:
    EventBus& bus;
    IAudioManager& audio;
};
