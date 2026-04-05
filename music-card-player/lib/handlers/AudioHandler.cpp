#include "AudioHandler.hpp"

AudioHandler::AudioHandler(EventBus& bus, IAudioManager& audioManager)
    : bus(bus)
    , audio(audioManager)
{
    bus.subscribe<MusicPlayPauseRequested>([&](const MusicPlayPauseRequested&) {
        if (!audio.hasStarted()) {
            audio.play("songs/white_zinfandel.mp3");
        } else {
            audio.pauseResume();
        }
    });

    bus.subscribe<VolumeChangeRequested>([&](const VolumeChangeRequested& e) {
        audio.setVolume(e.level);
    });

    bus.subscribe<MusicStopRequested>([&](const MusicStopRequested&) {
        audio.stop();
        bus.publish(OpenMainMenuStateRequested{});
    });
}
