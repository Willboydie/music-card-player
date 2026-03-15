#pragma once

#include "View.hpp"
#include "../screen/Screen.hpp"
#include <string>


class Renderer {
public:

    explicit Renderer(Screen& _screen);
    ~Renderer() = default;

    // Polymorphic entry point – clears the screen, then dispatches
    // to the correct handler based on the concrete View type.
    // Writes directly into screen.pixels.
    void render(const View& view);


    // ── View renderers ───────────────────────────────────────────
    void render(const MenuView& view);
    void render(const LoadingView& view);
    void render(const PlayerView& view);

private:
    Screen& screen;

    const int width;
    const int height;
    const int maxCharsPerLine;   // how many font cells fit in one row

    // ── Composite drawing components ─────────────────────────────
    void drawTitleBar(const std::string& title);
    void drawMenuItem(int y, const std::string& label,
                      bool selected, bool enabled);
    void drawPlayIcon(int cx, int cy);
    void drawPauseIcon(int cx, int cy);
    void drawProgressBar(int x, int y, int w, float fill);
    void drawDottedLine(int x, int y, int length);
    void drawStipple(int x, int y, int w, int h);
    void drawCentredText(int y, const std::string& text);

    // ── Text rendering ───────────────────────────────────────────
    void drawChar(int x, int y, char ch, bool inverted = false);
    void drawText(int x, int y, const std::string& text,
                  bool inverted = false, std::size_t maxChars = 0);

    // ── Pixel primitives (write directly to screen.pixels) ───────
    void setPixel(int x, int y, bool value = true);
    void drawHLine(int x, int y, int length);
    void drawVLine(int x, int y, int length);
    void drawRect (int x, int y, int w, int h);
    void fillRect (int x, int y, int w, int h);
    void clearRect(int x, int y, int w, int h);
};
