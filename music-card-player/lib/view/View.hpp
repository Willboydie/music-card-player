#pragma once

#include <string>
#include <vector>

struct View {
    virtual ~View() = default;
};


struct MenuView : View {
    struct Item {
        std::string label;
        bool enabled = true;
    };

    std::string title;
    std::vector<Item> items;
    int selectedIndex = 0;
};


struct LoadingView : View {
    std::string message;
};


struct PlayerView : View {
    std::string trackName;
    std::string artistName;
    bool isPlaying = false;
    float volume = 1.0f;
};
