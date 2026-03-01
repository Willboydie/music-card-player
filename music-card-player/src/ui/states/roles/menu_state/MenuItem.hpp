#pragma once

#include <functional>
#include <string>
#include "MenuOption.hpp"
#include "../../../event/Event.hpp"
#include "../../../event/EventBus.hpp"

struct MenuItem {
    MenuOption option;
    std::function<void(EventBus&)> onSelect;
    std::string label;
    bool enabled = true;
};