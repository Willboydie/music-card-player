#pragma once

#include "IAudioManager.hpp"
#include <string>
#include "../debugger/Debugger.hpp"
#include "../event/EventBus.hpp"

class MockAudioManager : public IAudioManager {
public:
    explicit MockAudioManager(EventBus& bus) : eventBus_(bus) {}
    ~MockAudioManager() override = default;

    MockAudioManager(const MockAudioManager&) = delete;
    MockAudioManager& operator=(const MockAudioManager&) = delete;

    bool initialise() override {
        Debugger::debug_msg("MockAudioManager: initialised");
        initialised_ = true;
        return true;
    }
    void shutdown() override {
        Debugger::debug_msg("MockAudioManager: shutting down");
        initialised_ = false;
    }

    bool play(const std::string& filepath) override {
        Debugger::debug_msg("MockAudioManager: playing " + filepath);
        playing_ = true;
        return true;
    }
    void pause() override {
        Debugger::debug_msg("MockAudioManager: paused");
        playing_ = false;
    }
    void resume() override {
        Debugger::debug_msg("MockAudioManager: resumed");
        playing_ = true;
    }
    void stop() override {
        Debugger::debug_msg("MockAudioManager: stopped");
        playing_ = false;
    }

    void  setVolume(float level) override {
        Debugger::debug_msg("MockAudioManager: set volume to " + std::to_string(level));
        volume_ = level;
    }
    float getVolume() const override { return volume_; }

    bool isPlaying() const override { return playing_; }
    bool isPaused() const override { return !playing_; }

private:
    EventBus& eventBus_;
    bool initialised_ = false;
    std::string currentTrack_ = "Baby by Justin Bieber.mp3";
    float volume_ = 0.0f;
    bool playing_ = false;
};