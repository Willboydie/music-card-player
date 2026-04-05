#pragma once

#include "State.hpp"

#include <algorithm>

class PlayerState : public State {
public:
    explicit PlayerState(EventBus& bus, Renderer& renderer, PlayerView& _view, std::string _name, Screen& _screen)
    : State(bus, renderer, _view, _name, _screen)
    , view(_view)
    {
        view.trackName = "White Zinfandel"; // TODO: get from music player
        view.artistName = "LAUSSE THE CAT";
        view.isPlaying = false;
        view.volume = 0.5f;
    }

    void onEvent(const UpButtonPressed&) override {
        view.volume = std::min(view.volume + 0.1f, 1.0f);
        renderer.render(view);
        screen.refresh();
        bus.publish(VolumeChangeRequested{ view.volume });
    }

    void onEvent(const DownButtonPressed&) override {
        view.volume = std::max(view.volume - 0.1f, 0.0f);
        renderer.render(view);
        screen.refresh();
        bus.publish(VolumeChangeRequested{ view.volume });
    }

    void onEvent(const SelectButtonPressed&) override {
        view.isPlaying = !view.isPlaying;
        renderer.render(view);
        screen.refresh();
        bus.publish(MusicPlayPauseRequested{});
    }

    void onEvent(const BackButtonPressed&) override {
        bus.publish(MusicStopRequested{});
    }

private:
    PlayerView& view;
};