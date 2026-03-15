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

    bool initialise() override;
    void shutdown() override;

    bool play(const std::string& filepath) override;
    void pauseResume();
    
    void stop() override;

    void  setVolume(float level) override;
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