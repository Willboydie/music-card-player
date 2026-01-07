# Makefile for Bluetooth Audio Connection
# Raspberry Pi Zero 2 W

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
PKG_CONFIG = pkg-config

# GLib/GIO flags for D-Bus communication
GLIB_CFLAGS = $(shell $(PKG_CONFIG) --cflags gio-2.0)
GLIB_LIBS = $(shell $(PKG_CONFIG) --libs gio-2.0)

# SDL2 flags for audio playback
SDL_CFLAGS = $(shell $(PKG_CONFIG) --cflags sdl2 SDL2_mixer)
SDL_LIBS = $(shell $(PKG_CONFIG) --libs sdl2 SDL2_mixer)

TARGETS = bluetooth_audio audio_player

all: $(TARGETS)

bluetooth_audio: bluetooth_audio.cpp
	$(CXX) $(CXXFLAGS) $(GLIB_CFLAGS) -o $@ $< $(GLIB_LIBS)

audio_player: audio_player.cpp
	$(CXX) $(CXXFLAGS) $(SDL_CFLAGS) -o $@ $< $(SDL_LIBS)

clean:
	rm -f $(TARGETS)

install: $(TARGETS)
	sudo cp $(TARGETS) /usr/local/bin/

.PHONY: all clean install

