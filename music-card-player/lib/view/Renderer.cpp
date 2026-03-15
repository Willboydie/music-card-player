#include "Renderer.hpp"
#include "font/Font.hpp"

Renderer::Renderer(Screen& _screen)
: screen(_screen)
, width(screen.getWidth())
, height(screen.getHeight())
, maxCharsPerLine(width / Font::CellWidth)
{}


// ── Polymorphic dispatch ─────────────────────────────────────────────────────

void Renderer::render(const View& view) {
    screen.pixels.reset();

    if (auto* v = dynamic_cast<const MenuView*>(&view)) {
        render(*v);
    } else if (auto* v = dynamic_cast<const LoadingView*>(&view)) {
        render(*v);
    } else if (auto* v = dynamic_cast<const PlayerView*>(&view)) {
        render(*v);
    }
}


// ── View renderers ───────────────────────────────────────────────────────────

void Renderer::render(const MenuView& view) {
    drawTitleBar(view.title);

    const int itemStartY = Font::CellHeight + 5;
    const int menuAreaHeight = height - itemStartY;
    clearRect(0, itemStartY, width, menuAreaHeight);
    // Explicitly clear menu area before drawing items (defensive for pixel persistence)
    clearRect(0, itemStartY, width, height - itemStartY);
    const int maxVisibleItems = (height - itemStartY) / Font::CellHeight;
    const int itemCount = static_cast<int>(view.items.size());

    // Clamp selectedIndex to valid range (handles stale index after items reload)
    int selectedIndex = view.selectedIndex;
    if (selectedIndex < 0 || selectedIndex >= itemCount) {
        selectedIndex = (itemCount > 0) ? 0 : -1;
    }

    // Scroll window: keep selectedIndex visible
    int scrollOffset = 0;
    if (selectedIndex >= maxVisibleItems) {
        scrollOffset = selectedIndex - maxVisibleItems + 1;
    }
    if (scrollOffset > itemCount - maxVisibleItems) {
        scrollOffset = itemCount - maxVisibleItems;
    }
    if (scrollOffset < 0) scrollOffset = 0;

    int end = scrollOffset + maxVisibleItems;
    if (end > itemCount) end = itemCount;

    // Clear entire menu content area so only the selected row gets white fill
    clearRect(0, itemStartY, width, height - itemStartY);

    for (int i = scrollOffset; i < end; ++i) {
        int row = i - scrollOffset;
        int iy = itemStartY + row * Font::CellHeight;

        bool selected = (i == selectedIndex);
        drawMenuItem(iy, view.items[i].label, selected, view.items[i].enabled);
    }
}

void Renderer::render(const LoadingView& view) {
    drawCentredText((height / 2) - Font::CellHeight, view.message);
    drawDottedLine(16, (height / 2) + 4, width - 32);
}

void Renderer::render(const PlayerView& view) {
    drawText(2, 2, view.trackName, false, maxCharsPerLine);
    drawText(2, 2 + Font::CellHeight + 2, view.artistName, false, maxCharsPerLine);

    int iconCX = width / 2;
    int iconCY = 33;
    const int iconSize = 12;
    clearRect(iconCX - iconSize / 2, iconCY - iconSize / 2, iconSize, iconSize);
    if (view.isPlaying) {
        drawPauseIcon(iconCX, iconCY);
    } else {
        drawPlayIcon(iconCX, iconCY);
    }

    drawProgressBar(8, height - 8, width - 16, view.volume);
}


// ── Composite drawing components ─────────────────────────────────────────────

void Renderer::drawTitleBar(const std::string& title) {
    drawHLine(0, 0, width);
    drawText(2, 2, title, false, maxCharsPerLine);
    drawHLine(0, Font::CellHeight + 3, width);
}

void Renderer::drawMenuItem(int y, const std::string& label,
                             bool selected, bool enabled) {
    if (selected) {
        fillRect(0, y, width, Font::CellHeight);
    } else {
        clearRect(0, y, width, Font::CellHeight);
    }

    drawText(2, y + 1, label, selected, maxCharsPerLine);

    if (!enabled) {
        drawStipple(0, y, width, Font::CellHeight);
    }
}

void Renderer::drawPlayIcon(int cx, int cy) {
    // Right-pointing triangle, 10 px tall, centred on (cx, cy)
    int x0 = cx - 5;
    int y0 = cy - 5;
    for (int row = 0; row < 10; ++row) {
        int halfWidth = (row < 5) ? row : (9 - row);
        drawHLine(x0, y0 + row, halfWidth + 1);
    }
}

void Renderer::drawPauseIcon(int cx, int cy) {
    // Two vertical bars, 10 px tall, centred on (cx, cy)
    int x0 = cx - 5;
    int y0 = cy - 5;
    fillRect(x0,     y0, 3, 10);
    fillRect(x0 + 6, y0, 3, 10);
}

void Renderer::drawProgressBar(int x, int y, int w, float fill) {
    clearRect(x, y, w, 4);
    drawRect(x - 1, y - 1, w + 2, 6);
    int filled = static_cast<int>(fill * w);
    fillRect(x, y, filled, 4);
}

void Renderer::drawDottedLine(int x, int y, int length) {
    for (int dx = 0; dx < length; dx += 4) {
        setPixel(x + dx, y);
        setPixel(x + dx + 1, y);
    }
}

void Renderer::drawStipple(int x, int y, int w, int h) {
    for (int dy = 0; dy < h; ++dy)
        for (int dx = 0; dx < w; dx += 2)
            setPixel(x + dx, y + dy, false);
}

void Renderer::drawCentredText(int y, const std::string& text) {
    int textX = (width - static_cast<int>(text.size()) * Font::CellWidth) / 2;
    if (textX < 0) textX = 0;
    drawText(textX, y, text, false, maxCharsPerLine);
}


// ── Text rendering ───────────────────────────────────────────────────────────

void Renderer::drawChar(int x, int y, char ch, bool inverted) {
    const uint8_t* g = Font::glyph(ch);
    for (int col = 0; col < Font::GlyphWidth; ++col) {
        uint8_t colBits = g[col];
        for (int row = 0; row < Font::GlyphHeight; ++row) {
            if (colBits & (1 << row)) {
                setPixel(x + col, y + row, !inverted);
            }
        }
    }
}

void Renderer::drawText(int x, int y, const std::string& text, bool inverted, std::size_t maxChars) {
    std::size_t limit = text.size();
    if (maxChars > 0 && maxChars < limit) {
        limit = maxChars;
    }
    for (std::size_t i = 0; i < limit; ++i) {
        drawChar(x + static_cast<int>(i) * Font::CellWidth, y,
                 text[i], inverted);
    }
}


// ── Pixel primitives (write directly to screen.pixels) ───────────────────────

void Renderer::setPixel(int x, int y, bool value) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    screen.pixels.set(y * width + x, value);
}

void Renderer::drawHLine(int x, int y, int length) {
    for (int i = 0; i < length; ++i)
        setPixel(x + i, y);
}

void Renderer::drawVLine(int x, int y, int length) {
    for (int i = 0; i < length; ++i)
        setPixel(x, y + i);
}

void Renderer::drawRect(int x, int y, int w, int h) {
    drawHLine(x,         y,         w);
    drawHLine(x,         y + h - 1, w);
    drawVLine(x,         y,         h);
    drawVLine(x + w - 1, y,         h);
}

void Renderer::fillRect(int x, int y, int w, int h) {
    for (int dy = 0; dy < h; ++dy)
        drawHLine(x, y + dy, w);
}

void Renderer::clearRect(int x, int y, int w, int h) {
    for (int dy = 0; dy < h; ++dy)
        for (int dx = 0; dx < w; ++dx)
            setPixel(x + dx, y + dy, false);
}
