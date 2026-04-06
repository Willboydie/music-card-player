# ─────────────────────────────────────────────────────────────
# Music Card Player – Raspberry Pi 5
# ─────────────────────────────────────────────────────────────

CXX        = g++
CXXFLAGS   = -Wall -Wextra -std=c++17 -O2 \
			 -march=armv8-a+crc -mtune=cortex-a53
PKG_CONFIG = pkg-config

TARGET     = music-card-player
BUILDDIR   = build
LIBDIR     = lib
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
	$(LIBDIR)/app_controller/AppController.cpp \
	$(LIBDIR)/audio/AudioManager.cpp \
	$(LIBDIR)/bluetooth/BluetoothManager.cpp \
	$(LIBDIR)/debugger/Debugger.cpp \
	$(LIBDIR)/storage/DeviceStorage.cpp \
	$(LIBDIR)/handlers/NavigationHandler.cpp \
	$(LIBDIR)/handlers/AudioHandler.cpp \
	$(LIBDIR)/handlers/BluetoothHandler.cpp \
	$(LIBDIR)/screen/Screen.cpp \
	$(LIBDIR)/view/Renderer.cpp \
	$(LIBDIR)/button/Button.cpp \
	$(LIBDIR)/button/ButtonListener.cpp \
	$(LIBDIR)/state_machine/StateMachine.cpp \
	$(LIBDIR)/state_machine/StateBuilder.cpp \
	$(LIBDIR)/states/MenuState.cpp \
	$(LIBDIR)/states/SavedDevicesMenu.cpp \
	$(LIBDIR)/states/FoundDevicesMenu.cpp \

# All headers use relative #include paths — no -I flags needed.

# ── Derived file lists ───────────────────────────────────────
# Object files mirror source structure: build/src/*.o and build/lib/**/*.o
SRC_OBJS  = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/$(SRCDIR)/%.o,$(filter $(SRCDIR)/%,$(SRCS)))
LIB_OBJS  = $(patsubst $(LIBDIR)/%.cpp,$(BUILDDIR)/$(LIBDIR)/%.o,$(filter $(LIBDIR)/%,$(SRCS)))
OBJS      = $(SRC_OBJS) $(LIB_OBJS)
DEPS      = $(OBJS:.o=.d)

ALL_CFLAGS = $(CXXFLAGS) $(GLIB_CFLAGS) $(SDL_CFLAGS) $(GPIOD_CFLAGS)
ALL_LIBS   = $(GLIB_LIBS) $(SDL_LIBS) $(GPIOD_LIBS)

# ── Rules ────────────────────────────────────────────────────

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(ALL_LIBS)

# Compile src/*.cpp → build/src/*.o
$(BUILDDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(ALL_CFLAGS) -MMD -MP -c -o $@ $<

# Compile lib/**/*.cpp → build/lib/**/*.o
$(BUILDDIR)/$(LIBDIR)/%.o: $(LIBDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(ALL_CFLAGS) -MMD -MP -c -o $@ $<

clean:
	rm -rf $(BUILDDIR) $(TARGET) run_button_test run_state_machine_test run_navigation_test run_audio_test

# ── Tests ─────────────────────────────────────────────────────────
TEST_SRCDIR = tests
# Production objs except main (each test provides its own main)
TEST_OBJS = $(filter-out $(BUILDDIR)/$(SRCDIR)/main.o,$(OBJS))

# Navigation test uses mocks instead of real Audio/Bluetooth managers
NAV_TEST_OBJS = $(filter-out $(BUILDDIR)/$(LIBDIR)/audio/AudioManager.o $(BUILDDIR)/$(LIBDIR)/bluetooth/BluetoothManager.o,$(TEST_OBJS)) \
                $(BUILDDIR)/$(LIBDIR)/audio/MockAudioManager.o \
                $(BUILDDIR)/$(LIBDIR)/bluetooth/MockBluetoothManager.o

TEST_INCLUDES = -I. -I$(SRCDIR) -I$(LIBDIR)

run_button_test: $(TEST_SRCDIR)/button_test.cpp $(TEST_OBJS)
	$(CXX) $(ALL_CFLAGS) $(TEST_INCLUDES) -o $@ $< $(TEST_OBJS) $(ALL_LIBS)

run_state_machine_test: $(TEST_SRCDIR)/state_machine_test.cpp $(TEST_OBJS)
	$(CXX) $(ALL_CFLAGS) $(TEST_INCLUDES) -o $@ $< $(TEST_OBJS) $(ALL_LIBS)

run_navigation_test: $(TEST_SRCDIR)/navigation_test.cpp $(NAV_TEST_OBJS)
	$(CXX) $(ALL_CFLAGS) $(TEST_INCLUDES) -o $@ $< $(NAV_TEST_OBJS) $(ALL_LIBS)

run_audio_test: $(TEST_SRCDIR)/audio_test.cpp $(TEST_OBJS)
	$(CXX) $(ALL_CFLAGS) $(TEST_INCLUDES) -o $@ $< $(TEST_OBJS) $(ALL_LIBS)

run_bluetooth_test: $(TEST_SRCDIR)/bluetooth_test.cpp $(TEST_OBJS)
	$(CXX) $(ALL_CFLAGS) $(TEST_INCLUDES) -o $@ $< $(TEST_OBJS) $(ALL_LIBS)

-include $(DEPS)

.PHONY: all clean run_button_test run_state_machine_test run_navigation_test run_audio_test
