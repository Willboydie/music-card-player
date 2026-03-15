#pragma once

#include "../screen/Screen.hpp"
#include "../event/EventBus.hpp"
#include "../event/Event.hpp"
#include "../view/Renderer.hpp"
#include "../view/View.hpp"
#include "../debugger/Debugger.hpp"
#include "../screen/Screen.hpp"

class State {
public:
    State(EventBus& _bus, Renderer& _renderer, View& _view, std::string _name, Screen& _screen) 
    : name(_name)
    , view(_view)
    , bus(_bus)
    , renderer(_renderer)
    , screen(_screen)
    {}

    std::string getName() const { return name; }

    virtual ~State() = default;

    // Default onEntry renders the view. Override and call State::onEntry()
    // at the end to keep the render, or call renderer.render(view) yourself.
    virtual void onEntry() { 
        renderer.render(view); 
        screen.refresh();
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

    std::string info;

private:
    std::string name;
    View& view;

protected:
    EventBus& bus;
    Renderer& renderer;
    Screen& screen;
};
