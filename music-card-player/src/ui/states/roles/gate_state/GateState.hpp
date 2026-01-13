#pragma once

#include "../State.hpp"
#include "../state_id/StateId.hpp"


class GateState : public State {
public:

    virtual bool isComplete() const = 0;
    virtual StateId destination() const = 0;
    virtual StateId launchState() const = 0;
    virtual bool isCancelled() const { return cancelled; }

private:
    bool cancelled = false;
};