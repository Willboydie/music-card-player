# ─────────────────────────────────────────────────────────────
# Music Card Player – Raspberry Pi 5
# ─────────────────────────────────────────────────────────────

CXX        = g++
CXXFLAGS   = -Wall -Wextra -std=c++17 -O2 \
             -march=armv8.2-a+crypto -mtune=cortex-a76
PKG_CONFIG = pkg-config

TARGET     = music-card-player
BUILDDIR   = build
SRCDIR     = src

# ── External library flags ───────────────────────────────────
GLIB_CFLAGS  = $(shell $(PKG_CONFIG) --cflags gio-2.0)
GLIB_LIBS    = $(shell $(PKG_CONFIG) --libs gio-2.0)

SDL_CFLAGS   = $(shell $(PKG_CONFIG) --cflags sdl2 SDL2_mixer)
SDL_LIBS     = $(shell $(PKG_CONFIG) --libs sdl2 SDL2_mixer)

GPIOD_CFLAGS = $(shell $(PKG_CONFIG) --cflags libgpiod 2>/dev/null)
GPIOD_LIBS   = $(shell $(PKG_CONFIG) --libs libgpiod 2>/dev/null || echo "-lgpiod")

# ── Source files ─────────────────────────────────────────────
SRCS = \
	$(SRCDIR)/main.cpp \
	$(SRCDIR)/app_controller/AppController.cpp \
	$(SRCDIR)/audio/AudioManager.cpp \
	$(SRCDIR)/bluetooth/BluetoothManager.cpp \
	$(SRCDIR)/debugger/Debugger.cpp \
	$(SRCDIR)/storage/DeviceStorage.cpp \
	$(SRCDIR)/handlers/NavigationHandler.cpp \
	$(SRCDIR)/handlers/AudioHandler.cpp \
	$(SRCDIR)/handlers/BluetoothHandler.cpp \
	$(SRCDIR)/ui/screen/Screen.cpp \
	$(SRCDIR)/ui/view/Renderer.cpp \
	$(SRCDIR)/ui/button/Button.cpp \
	$(SRCDIR)/ui/button/ButtonListener.cpp \
	$(SRCDIR)/ui/state_machine/StateMachine.cpp \
	$(SRCDIR)/ui/state_machine/StateBuilder.cpp \
	$(SRCDIR)/ui/states/roles/menu_state/MenuState.cpp \
	$(SRCDIR)/ui/states/bluetooth/saved_devices/SavedDevicesMenu.cpp \
	$(SRCDIR)/ui/states/bluetooth/connect_new/found_devices/FoundDevicesMenu.cpp

# All headers use relative #include paths — no -I flags needed.

# ── Derived file lists ───────────────────────────────────────
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

ALL_CFLAGS = $(CXXFLAGS) $(GLIB_CFLAGS) $(SDL_CFLAGS) $(GPIOD_CFLAGS)
ALL_LIBS   = $(GLIB_LIBS) $(SDL_LIBS) $(GPIOD_LIBS)

# ── Rules ────────────────────────────────────────────────────

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(ALL_LIBS)

# Compile each .cpp → .o, auto-generating header dependencies
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(ALL_CFLAGS) -MMD -MP -c -o $@ $<

clean:
	rm -rf $(BUILDDIR) $(TARGET) run_button_test run_state_machine_test run_navigation_test

# ── Tests ─────────────────────────────────────────────────────────
TEST_SRCDIR = tests
# Production objs except main (each test provides its own main)
TEST_OBJS = $(filter-out $(BUILDDIR)/main.o,$(OBJS))

run_button_test: $(TEST_SRCDIR)/button_test.cpp $(TEST_OBJS)
	$(CXX) $(ALL_CFLAGS) -I$(SRCDIR) -o $@ $< $(TEST_OBJS) $(ALL_LIBS)

run_state_machine_test: $(TEST_SRCDIR)/state_machine_test.cpp $(TEST_OBJS)
	$(CXX) $(ALL_CFLAGS) -I$(SRCDIR) -o $@ $< $(TEST_OBJS) $(ALL_LIBS)

run_navigation_test: $(TEST_SRCDIR)/navigation_test.cpp $(TEST_OBJS)
	$(CXX) $(ALL_CFLAGS) -I$(SRCDIR) -o $@ $< $(TEST_OBJS) $(ALL_LIBS)

-include $(DEPS)

.PHONY: all clean run_button_test run_state_machine_test run_navigation_test
