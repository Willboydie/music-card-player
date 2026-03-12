#pragma once

#include <functional>
#include <string>
#include "../../../../event/Event.hpp"
#include "../../../../event/EventBus.hpp"

struct MenuItem {
    std::function<void(EventBus&)> onSelect;
    std::string label;
    bool enabled = true;
};