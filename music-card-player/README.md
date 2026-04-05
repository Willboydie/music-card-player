# Music Card Player

A C++ application for Raspberry Pi that plays music via Bluetooth headphones, controlled through physical push buttons and an SSD1106 128×64 I2C OLED display.

## How It Works

```
┌────────────┐     ┌──────────┐     ┌────────────┐     ┌─────────────┐
│  Buttons   │ ──▶ │ EventBus │ ──▶ │   State    │ ──▶ │   OLED      │
│  (GPIO)    │     │          │     │  Machine   │     │  Display    │
└────────────┘     └──────────┘     └────────────┘     └─────────────┘
                        │
              ┌─────────┼─────────┐
              ▼         ▼         ▼
        ┌──────────┐ ┌───────┐ ┌──────────────┐
        │ Bluetooth│ │ Audio │ │  Navigation  │
        │ Manager  │ │Manager│ │   Handler    │
        └──────────┘ └───────┘ └──────────────┘
```

1. **Buttons** detect presses via GPIO and publish events to the EventBus
2. **NavigationHandler** forwards button events to the current state and handles screen transitions
3. **AudioHandler** responds to play/pause/volume events via SDL2 + PulseAudio
4. **BluetoothHandler** manages device discovery, pairing, and connection via BlueZ D-Bus
5. **Renderer** draws the current view (menus, player, loading screens) to the OLED display

## Supported Hardware

| Board | SoC | Architecture | Status |
|---|---|---|---|
| Raspberry Pi 5 | BCM2712 | aarch64 (Cortex-A76) | Primary target |
| Raspberry Pi Zero 2 W | BCM2710A1 | aarch64 (Cortex-A53) | Supported |

Both boards share the same aarch64 instruction set. The default Makefile targets the Pi 5; see [Building](#building) for Pi Zero 2 W instructions.

### Peripherals

- **Display**: SSD1106 128×64 I2C OLED (address `0x3C` on `/dev/i2c-1`)
- **Buttons**: 4× momentary push buttons, each connecting a GPIO pin to GND when pressed
- **Audio**: Bluetooth headphones/speakers via A2DP (BlueZ + PulseAudio)

## Prerequisites

Install required packages on your Raspberry Pi:

```bash
sudo apt update
sudo apt install -y \
    build-essential \
    bluez bluez-tools \
    pulseaudio pulseaudio-module-bluetooth \
    libglib2.0-dev \
    libsdl2-dev libsdl2-mixer-dev \
    libgpiod-dev \
    i2c-tools
```

Enable I2C if you haven't already:

```bash
sudo raspi-config nonint do_i2c 0
```

Verify the OLED display is detected:

```bash
i2cdetect -y 1
# Should show 0x3C in the grid
```

## Deploying from Windows

From PowerShell on your development machine:

```powershell
.\deploy.ps1 user@your-pi-hostname.local
```

This copies the entire project to `~/music-card-player` on the Pi via `scp`.

### Song Upload (for prototyping)
Use
```powershell
scp -r songs user@your-pi-username.local:music-card-player
```
to upload the song folder to the pi.

## Building

SSH into your Pi, then:

```bash
cd ~/music-card-player
```

### Raspberry Pi 5 (default)

```bash
make -j4
```

The default Makefile uses `-march=armv8.2-a+crypto -mtune=cortex-a76` which is optimal for the Pi 5.

### Raspberry Pi Zero 2 W

Override the architecture flags for the Cortex-A53:

```bash
make -j4 CXXFLAGS="-Wall -Wextra -std=c++17 -O2 -march=armv8-a+crc -mtune=cortex-a53"
```

### Clean rebuild

```bash
make clean
make -j4
```

## Running

```bash
./music-card-player
```

The application starts in the main menu. Use the four physical buttons (up, down, select, back) to navigate.

## Project Structure

```
src/
├── main.cpp                          # Entry point
├── app_controller/                   # Top-level wiring (EventBus, managers, handlers)
├── audio/                            # AudioManager (SDL2_mixer)
├── bluetooth/                        # BluetoothManager (BlueZ D-Bus), BluetoothDevice
├── storage/                          # DeviceStorage (flat-file persistence)
├── handlers/                         # Event handlers
│   ├── NavigationHandler             #   Button + navigation events → StateMachine
│   ├── AudioHandler                  #   Play/pause/volume events → AudioManager
│   └── BluetoothHandler              #   Search/connect events → BluetoothManager
├── utils/                            # Constants (screen dimensions, font size)
└── ui/
    ├── event/                        # Event structs and EventBus (pub/sub)
    ├── screen/                       # Screen (SSD1106 I2C driver)
    ├── view/                         # View structs, Renderer, Font
    ├── button/                       # Button (GPIO debounce), ButtonListener
    ├── state_machine/                # StateMachine, StateBuilder, StateId
    └── states/                       # State base class + concrete states
        ├── main_menu/
        ├── player/
        ├── saved_devices/
        ├── connect_new/
        ├── found_devices/
        ├── searching_for_devices/
        ├── bluetooth_connecting/
```

## Wireless Audio Setup

```
┌─────────────┐      ┌─────────────┐      ┌──────────────┐      ┌─────────────┐
│  Your App   │ ──▶  │ PulseAudio  │ ──▶ │   BlueZ      │ ──▶  │ Headphones  │
│ (SDL2/ALSA) │      │  (Router)   │      │ (Bluetooth)  │      │   (A2DP)    │
└─────────────┘      └─────────────┘      └──────────────┘      └─────────────┘
```

After connecting via the on-screen Bluetooth menu, you may need to configure PulseAudio:

```bash
# List audio sinks
pactl list sinks short

# Set Bluetooth as default output
pactl set-default-sink bluez_sink.AA_BB_CC_DD_EE_FF.a2dp_sink
```

## Auto-Start on Boot

Create a systemd service:

```bash
sudo tee /etc/systemd/system/music-card-player.service > /dev/null <<'EOF'
[Unit]
Description=Music Card Player
After=bluetooth.target graphical.target

[Service]
Type=simple
User=rory
ExecStart=/home/rory/music-card-player/music-card-player
Restart=on-failure
RestartSec=5

[Install]
WantedBy=multi-user.target
EOF

sudo systemctl enable --now music-card-player.service
```

## Troubleshooting

### Bluetooth adapter not found

```bash
rfkill list
sudo rfkill unblock bluetooth
sudo systemctl restart bluetooth
```

### No audio after connection

```bash
pulseaudio -k
pulseaudio --start
pactl list cards
```

### Permission denied

```bash
sudo usermod -a -G bluetooth,gpio,i2c $USER
# Log out and back in
```

### OLED display not responding

```bash
# Check I2C bus
i2cdetect -y 1

# Should show 0x3C. If not, check wiring and that I2C is enabled.
```

### GPIO buttons not working (Pi 5)

The Pi 5 uses a different GPIO chip (`/dev/gpiochip0`) than earlier models like the Zero 2 W (`/dev/gpiochip4`). If buttons aren't detected, check that `ButtonListener.cpp` references the correct chip for your board.