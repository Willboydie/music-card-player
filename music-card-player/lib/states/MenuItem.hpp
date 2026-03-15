#pragma once

#include <functional>
#include <string>

struct MenuItem {
    std::function<void(EventBus&)> onSelect;
    std::string label;
    bool enabled = true;
};