/**
 * Audio Player for Bluetooth Headphones
 * Raspberry Pi Zero 2 W
 * 
 * Uses SDL2 for cross-platform audio playback over Bluetooth
 * 
 * Build: g++ -o audio_player audio_player.cpp $(pkg-config --cflags --libs sdl2 SDL2_mixer)
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>

class AudioPlayer {
private:
    bool initialized;
    Mix_Music* current_music;
    
public:
    AudioPlayer() : initialized(false), current_music(nullptr) {}
    
    ~AudioPlayer() {
        cleanup();
    }
    
    bool initialize() {
        // Initialize SDL audio subsystem
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
            return false;
        }
        
        // Initialize SDL_mixer with common audio settings
        // 44100 Hz, 16-bit stereo, 2048 byte chunks
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer Init failed: " << Mix_GetError() << std::endl;
            SDL_Quit();
            return false;
        }
        
        // Enable MP3 and other format support
        int flags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC;
        if ((Mix_Init(flags) & flags) != flags) {
            std::cerr << "Warning: Some audio formats may not be supported" << std::endl;
        }
        
        initialized = true;
        std::cout << "Audio system initialized" << std::endl;
        return true;
    }
    
    bool playFile(const std::string& filepath) {
        if (!initialized) {
            std::cerr << "Audio not initialized" << std::endl;
            return false;
        }
        
        // Stop any currently playing music
        stop();
        
        // Load the audio file
        current_music = Mix_LoadMUS(filepath.c_str());
        if (!current_music) {
            std::cerr << "Failed to load: " << filepath << std::endl;
            std::cerr << "Error: " << Mix_GetError() << std::endl;
            return false;
        }
        
        // Play the music (-1 = loop forever, 0 = play once)
        if (Mix_PlayMusic(current_music, 0) < 0) {
            std::cerr << "Failed to play: " << Mix_GetError() << std::endl;
            return false;
        }
        
        std::cout << "Now playing: " << filepath << std::endl;
        return true;
    }
    
    void pause() {
        if (Mix_PlayingMusic()) {
            Mix_PauseMusic();
            std::cout << "Paused" << std::endl;
        }
    }
    
    void resume() {
        if (Mix_PausedMusic()) {
            Mix_ResumeMusic();
            std::cout << "Resumed" << std::endl;
        }
    }
    
    void stop() {
        Mix_HaltMusic();
        if (current_music) {
            Mix_FreeMusic(current_music);
            current_music = nullptr;
        }
    }
    
    void setVolume(int volume) {
        // Volume: 0-128
        volume = std::max(0, std::min(128, volume));
        Mix_VolumeMusic(volume);
        std::cout << "Volume: " << (volume * 100 / 128) << "%" << std::endl;
    }
    
    bool isPlaying() {
        return Mix_PlayingMusic() && !Mix_PausedMusic();
    }
    
    void waitForFinish() {
        while (Mix_PlayingMusic()) {
            SDL_Delay(100);
        }
    }
    
    void cleanup() {
        stop();
        Mix_CloseAudio();
        Mix_Quit();
        SDL_Quit();
        initialized = false;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <audio_file.mp3>" << std::endl;
        std::cout << "\nSupported formats: MP3, OGG, FLAC, WAV" << std::endl;
        return 1;
    }
    
    AudioPlayer player;
    
    if (!player.initialize()) {
        return 1;
    }
    
    // Set volume to 80%
    player.setVolume(100);
    
    // Play the file
    if (!player.playFile(argv[1])) {
        return 1;
    }
    
    std::cout << "\nControls:" << std::endl;
    std::cout << "  p - Pause/Resume" << std::endl;
    std::cout << "  +/- - Volume up/down" << std::endl;
    std::cout << "  q - Quit" << std::endl;
    
    int volume = 100;
    
    // Simple control loop
    while (player.isPlaying() || Mix_PausedMusic()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto done;
            }
        }
        
        // Non-blocking key check (basic stdin polling)
        fd_set fds;
        struct timeval tv = {0, 100000}; // 100ms timeout
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        
        if (select(1, &fds, nullptr, nullptr, &tv) > 0) {
            char c;
            if (read(0, &c, 1) > 0) {
                switch (c) {
                    case 'p':
                        if (Mix_PausedMusic()) {
                            player.resume();
                        } else {
                            player.pause();
                        }
                        break;
                    case '+':
                    case '=':
                        volume = std::min(128, volume + 10);
                        player.setVolume(volume);
                        break;
                    case '-':
                        volume = std::max(0, volume - 10);
                        player.setVolume(volume);
                        break;
                    case 'q':
                        goto done;
                }
            }
        }
        
        SDL_Delay(50);
    }
    
done:
    std::cout << "\nPlayback finished" << std::endl;
    return 0;
}

