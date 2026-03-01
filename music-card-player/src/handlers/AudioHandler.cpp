#include "AudioHandler.hpp"

AudioHandler::AudioHandler(EventBus& bus, AudioManager& audioManager)
    : bus(bus)
    , audio(audioManager)
{
    bus.subscribe<MusicPlayRequested>([&](const MusicPlayRequested&) {
        audio.resume();
    });

    bus.subscribe<MusicPauseRequested>([&](const MusicPauseRequested&) {
        audio.pause();
    });

    bus.subscribe<VolumeChangeRequested>([&](const VolumeChangeRequested& e) {
        audio.setVolume(e.level);
    });
}
