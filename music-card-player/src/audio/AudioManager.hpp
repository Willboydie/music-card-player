#pragma once

#include "IAudioManager.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include "../debugger/Debugger.hpp"
#include "../event/EventBus.hpp"

class AudioManager : public IAudioManager {
public:
    explicit AudioManager(EventBus& bus);
    ~AudioManager() override;

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    bool initialise() override;
    void shutdown() override;
    bool play(const std::string& filepath) override;
    void pause() override;
    void resume() override;
    void stop() override;
    void  setVolume(float level) override;
    float getVolume() const override;
    bool isPlaying() const override;
    bool isPaused() const override;

private:
    static constexpr int MaxVolume = MIX_MAX_VOLUME;  // 128

    EventBus& eventBus_;
    bool initialised;
    Mix_Music* currentTrack;
    float volume;

    int toSdlVolume(float level) const;
};
