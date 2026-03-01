#pragma once

#include <string>

class IAudioManager {
public:
    virtual ~IAudioManager() = default;

    // ── Lifecycle ────────────────────────────────────────────────
    virtual bool initialise() = 0;
    virtual void shutdown() = 0;

    // ── Playback ─────────────────────────────────────────────────
    virtual bool play(const std::string& filepath) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;

    // ── Volume (0.0 – 1.0) ──────────────────────────────────────
    virtual void  setVolume(float level) = 0;
    virtual float getVolume() const = 0;

    // ── Status ───────────────────────────────────────────────────
    virtual bool isPlaying() const = 0;
    virtual bool isPaused() const = 0;
};
