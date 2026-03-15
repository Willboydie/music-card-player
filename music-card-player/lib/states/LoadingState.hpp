#pragma once

#include "State.hpp"

// Base for states that use a shared LoadingView. Sets view.message on entry
// so the correct message is shown regardless of construction order.
class LoadingState : public State {
public:
    explicit LoadingState(EventBus& bus, Renderer& renderer, LoadingView& _view,
                         std::string _name, Screen& _screen)
    : State(bus, renderer, _view, _name, _screen)
    , view(_view)
    {}

    void onEntry() override {
        view.message = loadingMessage;
        State::onEntry();
    }

protected:
    LoadingView& view;
    std::string loadingMessage;
};
