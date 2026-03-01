#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

// Manages audio playback via SDL2_mixer.
//
// Typical usage:
//     AudioManager audio;
//     audio.initialise();
//     audio.setVolume(0.8f);
//     audio.play("/path/to/track.mp3");
//     audio.pause();
//     audio.resume();
//     audio.stop();

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // Non-copyable
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // ── Lifecycle ────────────────────────────────────────────────
    bool initialise();
    void shutdown();

    // ── Playback ─────────────────────────────────────────────────
    bool play(const std::string& filepath);
    void pause();
    void resume();
    void stop();

    // ── Volume (0.0 – 1.0) ──────────────────────────────────────
    void  setVolume(float level);
    float getVolume() const;

    // ── Status ───────────────────────────────────────────────────
    bool isPlaying() const;
    bool isPaused() const;

private:
    static constexpr int MaxVolume = MIX_MAX_VOLUME;  // 128

    bool initialised;
    Mix_Music* currentTrack;
    float volume;

    int toSdlVolume(float level) const;
};
