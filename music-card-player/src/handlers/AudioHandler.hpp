#pragma once

#include "../event/EventBus.hpp"
#include "../event/Event.hpp"
#include "../audio/IAudioManager.hpp"

class AudioHandler {
public:
    AudioHandler(EventBus& bus, IAudioManager& audioManager);

private:
    EventBus& bus;
    IAudioManager& audio;
};
