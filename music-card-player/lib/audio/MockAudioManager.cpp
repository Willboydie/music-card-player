#include "MockAudioManager.hpp"

bool MockAudioManager::initialise() {
    Debugger::debug_msg("MockAudioManager: initialised");
    initialised_ = true;
    return true;
}
void MockAudioManager::shutdown() {
    Debugger::debug_msg("MockAudioManager: shutting down");
    initialised_ = false;
}

bool MockAudioManager::play(const std::string& filepath) {
    Debugger::debug_msg("MockAudioManager: playing " + filepath);
    playing_ = true;
    return true;
}
void MockAudioManager::pauseResume() {
    if (playing_) {
        Debugger::debug_msg("MockAudioManager: paused");
    } else {
        Debugger::debug_msg("MockAudioManager: resumed");
    }
    playing_ = !playing_;
}

void MockAudioManager::stop() {
    Debugger::debug_msg("MockAudioManager: stopped");
    playing_ = false;
}

void MockAudioManager::setVolume(float level) {
    Debugger::debug_msg("MockAudioManager: set volume to " + std::to_string(level));
    volume_ = level;
}