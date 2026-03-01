#pragma once

#include "../State.hpp"
#include <algorithm>

class PlayerState : public State {
public:
    explicit PlayerState(EventBus& bus, Renderer& renderer, PlayerView& _view, std::string _name)
    : State(bus, renderer, _view, _name)
    , view(_view)
    {
        view.trackName = "Track Name"; // TODO: get from music player
        view.artistName = "Artist Name";
        view.isPlaying = true;
        view.volume = 0.5f;
    }

    void onEvent(const UpButtonPressed&) override {
        view.volume = std::min(view.volume + 0.1f, 1.0f);
        renderer.render(view);
        bus.publish(VolumeChangeRequested{ view.volume });
    }

    void onEvent(const DownButtonPressed&) override {
        view.volume = std::max(view.volume - 0.1f, 0.0f);
        renderer.render(view);
        bus.publish(VolumeChangeRequested{ view.volume });
    }

    void onEvent(const SelectButtonPressed&) override {
        view.isPlaying = !view.isPlaying;
        renderer.render(view);
        bus.publish(MusicPlayRequested{});
    }

    void onEvent(const BackButtonPressed&) override {
        bus.publish(BackNavigationRequested{});
    }

private:
    PlayerView& view;
};