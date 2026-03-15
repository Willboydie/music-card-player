#include "AudioHandler.hpp"

AudioHandler::AudioHandler(EventBus& bus, IAudioManager& audioManager)
    : bus(bus)
    , audio(audioManager)
{
    bus.subscribe<MusicPlayRequested>([&](const MusicPlayRequested&) {
        if (!audio.started) {
            audio.play("../../../songs/white_zinfandel.mp3");
        } else {
            audio.pauseResume();
        }
    });

    bus.subscribe<MusicPauseRequested>([&](const MusicPauseRequested&) {
        audio.pauseResume();
    });

    bus.subscribe<VolumeChangeRequested>([&](const VolumeChangeRequested& e) {
        audio.setVolume(e.level);
    });

    bus.subscribe<MusicStopRequested>([&](const MusicStopRequested&) {
        audio.stop();
        bus.publish(OpenMainMenuStateRequested{});
    });
}
