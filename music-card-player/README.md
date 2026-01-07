# Bluetooth Audio Connection for Raspberry Pi Zero 2 W

Basic C++ boilerplate for connecting to Bluetooth headphones and playing audio using BlueZ D-Bus API and SDL2.

## How Wireless Audio Works

```
┌─────────────┐      ┌─────────────┐      ┌──────────────┐      ┌─────────────┐
│  Your App   │ ──▶  │ PulseAudio  │ ──▶  │   BlueZ      │ ──▶  │ Headphones  │
│ (SDL2/ALSA) │      │  (Router)   │      │ (Bluetooth)  │      │   (A2DP)    │
└─────────────┘      └─────────────┘      └──────────────┘      └─────────────┘
```

1. **Your app** plays audio using SDL2 (or any audio library)
2. **PulseAudio** receives the audio and routes it to the active output
3. **BlueZ** transmits audio wirelessly via A2DP profile
4. **Headphones** receive and play the audio

## Prerequisites

Install required packages on your Raspberry Pi:

```bash
sudo apt update
sudo apt install -y \
    bluez bluez-tools \
    pulseaudio pulseaudio-module-bluetooth \
    libglib2.0-dev \
    libsdl2-dev libsdl2-mixer-dev
```

## Building

```bash
cd bluetooth
make
```

Or compile directly:

```bash
g++ -o bluetooth_audio bluetooth_audio.cpp $(pkg-config --cflags --libs gio-2.0)
```

## Usage

### Interactive Mode

```bash
./bluetooth_audio
```

This will:
1. Power on the Bluetooth adapter
2. Scan for devices for 10 seconds
3. List discovered devices
4. Prompt you to select a device to connect

### Direct Connection

If you know your headphones' MAC address:

```bash
./bluetooth_audio AA:BB:CC:DD:EE:FF
```

## Playing Audio to Bluetooth Headphones

### Using the Audio Player

```bash
# Build the audio player
make audio_player

# Play an MP3 file (will output to connected Bluetooth headphones)
./audio_player /path/to/song.mp3
```

Controls while playing:
- `p` - Pause/Resume
- `+` / `-` - Volume up/down
- `q` - Quit

### Quick Test with Command Line

```bash
# Test with a WAV file
aplay -D bluealsa test.wav

# Play MP3 with mpg123
mpg123 song.mp3

# Play any format with ffplay
ffplay -nodisp -autoexit song.mp3
```

## Setting Up Audio

After connecting, you may need to configure PulseAudio to use the Bluetooth device:

```bash
# List audio sinks
pactl list sinks short

# Set Bluetooth as default sink
pactl set-default-sink bluez_sink.AA_BB_CC_DD_EE_FF.a2dp_sink
```

## Auto-Connect on Boot

Create a systemd service for automatic connection:

```bash
sudo nano /etc/systemd/system/bluetooth-headphones.service
```

```ini
[Unit]
Description=Connect Bluetooth Headphones
After=bluetooth.target

[Service]
Type=oneshot
ExecStart=/usr/local/bin/bluetooth_audio AA:BB:CC:DD:EE:FF
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
```

Enable the service:

```bash
sudo systemctl enable bluetooth-headphones.service
```

## Troubleshooting

### Bluetooth adapter not found
```bash
# Check if Bluetooth is blocked
rfkill list
sudo rfkill unblock bluetooth

# Restart Bluetooth service
sudo systemctl restart bluetooth
```

### No audio after connection
```bash
# Restart PulseAudio
pulseaudio -k
pulseaudio --start

# Check Bluetooth audio profile
pactl list cards
```

### Permission denied
```bash
# Add user to bluetooth group
sudo usermod -a -G bluetooth $USER
# Then log out and back in
```

