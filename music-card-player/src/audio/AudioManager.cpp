#include "AudioManager.hpp"
#include "../ui/event/Event.hpp"
#include <algorithm>
#include <iostream>


// ── Lifecycle ────────────────────────────────────────────────────────────────

AudioManager::AudioManager(EventBus& bus)
    : eventBus_(bus)
    , initialised(false)
    , currentTrack(nullptr)
    , volume(1.0f)
{}

AudioManager::~AudioManager() {
    shutdown();
}

bool AudioManager::initialise() {
    Debugger::debug_msg("AudioManager: initialising");
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "AudioManager: SDL init failed: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    // 44100 Hz, 16-bit stereo, 2048 byte chunks
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "AudioManager: mixer init failed: "
                  << Mix_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    int flags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC;
    if ((Mix_Init(flags) & flags) != flags) {
        std::cerr << "AudioManager: some audio formats may not be supported"
                  << std::endl;
    }

    initialised = true;
    Debugger::debug_msg("AudioManager: initialised");
    return true;
}

void AudioManager::shutdown() {
    if (!initialised) return;
    Debugger::debug_msg("AudioManager: shutting down");

    stop();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    initialised = false;
}


// ── Playback ─────────────────────────────────────────────────────────────────

bool AudioManager::play(const std::string& filepath) {
    if (!initialised) return false;
    Debugger::debug_msg("AudioManager: playing " + filepath);
    stop();

    currentTrack = Mix_LoadMUS(filepath.c_str());
    if (!currentTrack) {
        std::cerr << "AudioManager: failed to load " << filepath
                  << " – " << Mix_GetError() << std::endl;
        return false;
    }

    if (Mix_PlayMusic(currentTrack, 0) < 0) {
        std::cerr << "AudioManager: playback failed – "
                  << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

void AudioManager::pause() {
    if (isPlaying()) {
        Mix_PauseMusic();
        Debugger::debug_msg("AudioManager: paused");
    }
}

void AudioManager::resume() {
    if (isPaused()) {
        Mix_ResumeMusic();
        Debugger::debug_msg("AudioManager: resumed");
    }
}

void AudioManager::stop() {
    Mix_HaltMusic();
    if (currentTrack) {
        Mix_FreeMusic(currentTrack);
        currentTrack = nullptr;
    }
    Debugger::debug_msg("AudioManager: stopped");
}


// ── Volume ───────────────────────────────────────────────────────────────────

void AudioManager::setVolume(float level) {
    volume = std::clamp(level, 0.0f, 1.0f);
    Mix_VolumeMusic(toSdlVolume(volume));
    Debugger::debug_msg("AudioManager: set volume to " + std::to_string(volume));
}

float AudioManager::getVolume() const {
    return volume;
}

int AudioManager::toSdlVolume(float level) const {
    return static_cast<int>(level * MaxVolume);
}


// ── Status ───────────────────────────────────────────────────────────────────

bool AudioManager::isPlaying() const {
    return Mix_PlayingMusic() && !Mix_PausedMusic();
}

bool AudioManager::isPaused() const {
    return Mix_PausedMusic() != 0;
}
