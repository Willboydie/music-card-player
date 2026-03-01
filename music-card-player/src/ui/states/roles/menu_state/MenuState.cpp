#include "MenuState.hpp"

void MenuState::syncView() {
    view.items.clear();
    view.items.reserve(items.size());
    for (const auto& mi : items) {
        view.items.push_back({ mi.label, mi.enabled });
    }
    view.selectedIndex = selectedIndex;
}

void MenuState::moveSelection(int delta) {
    const int count = static_cast<int>(items.size());
    if (count == 0) return;

    do {
        selectedIndex = (selectedIndex + delta + count) % count;
    } while (!items[selectedIndex].enabled);

    view.selectedIndex = selectedIndex;
    renderer.render(view);
}

void MenuState::ensureValidSelection() {
    if (items.empty()) return;

    if (!items[selectedIndex].enabled) {
        moveSelection(1);
    }
}