#pragma once

#include "../screen/Screen.hpp"
#include "../../event/EventBus.hpp"
#include "../../event/Event.hpp"
#include "../view/Renderer.hpp"
#include "../view/View.hpp"
#include "../../debugger/Debugger.hpp"


class State {
public:
    State(EventBus& _bus, Renderer& _renderer, View& _view, std::string _name) 
    : bus(_bus)
    , renderer(_renderer)
    , view(_view)
    , name(_name)
    { }

    std::string getName() const { return name; }

    virtual ~State() = default;

    // Default onEntry renders the view. Override and call State::onEntry()
    // at the end to keep the render, or call renderer.render(view) yourself.
    virtual void onEntry() { 
        renderer.render(view); 
        Debugger::debug_msg("Entered state: " + name);
    }
    virtual void onExit() {
        Debugger::debug_msg("Exited state: " + name);
    }

    // Physical event handlers
    // These decide on the intention of the user (if any) and emit that event
    virtual void onEvent(const UpButtonPressed&) { }
    virtual void onEvent(const DownButtonPressed&) { }
    virtual void onEvent(const SelectButtonPressed&) { }
    virtual void onEvent(const BackButtonPressed&) { }

    // Event handlers must always be called "onEvent" with different input event types.
    // This allows the StateMachine to call them generically.

protected:
    EventBus& bus;
    Renderer& renderer;

private:
    View& view;
    std::string name;
};
