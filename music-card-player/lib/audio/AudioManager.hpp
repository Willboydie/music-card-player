#pragma once

#include "IAudioManager.hpp"
#include "../debugger/Debugger.hpp"
#include "../event/EventBus.hpp"
#include "../event/Event.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <algorithm>
#include <iostream>

class AudioManager : public IAudioManager {
public:
    explicit AudioManager(EventBus& bus);
    ~AudioManager() override;

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    bool initialise() override;
    void shutdown() override;

    bool play(const std::string& filepath) override;
    void pauseResume() override;
    void stop() override;

    void  setVolume(float level) override;
    float getVolume() const override;
    
    bool isPlaying() const override;
    bool isPaused() const override;

    bool hasStarted() const override { return started; }

private:
    static constexpr int MaxVolume = MIX_MAX_VOLUME;  // 128

    EventBus& eventBus_;
    bool sdlReady = false;
    bool initialised = false;
    Mix_Music* currentTrack;
    float volume;
    bool started = false;

    bool openMixer();
    int toSdlVolume(float level) const;
};
