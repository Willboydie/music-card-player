# Tools

## `BT.py` — nearby Bluetooth audio device scan (Raspberry Pi / BlueZ)

This script talks to **BlueZ** over the **system D-Bus** on the Raspberry Pi (including Pi 5). It starts a **10 second** inquiry/discovery pass, then prints **one line per nearby device name** that looks like an **audio** device (A2DP / headset-style UUIDs, or Audio/Video class of device).

### Prerequisites

- Raspberry Pi OS (or another Debian-based image) with **BlueZ** running
- Python 3 and the D-Bus bindings for Python

Install packages:

```bash
sudo apt update
sudo apt install -y python3-dbus bluez
```

Enable and start Bluetooth (usually already on):

```bash
sudo systemctl enable --now bluetooth
```

### Permissions

The script uses the **system** bus (`org.bluez`). On Raspberry Pi OS you can usually run it as a normal user if that user can talk to BlueZ. If you see permission errors:

1. Add your user to the `bluetooth` group, then log out and back in:

   ```bash
   sudo usermod -aG bluetooth "$USER"
   ```

2. Or run once with elevated privileges:

   ```bash
   sudo python3 BT.py
   ```

### Run

From this directory:

```bash
python3 BT.py
```

Or from elsewhere:

```bash
python3 /path/to/music-card-player/tools/BT.py
```

Successful output is a sorted list of **device names** (alias/name), one per line, written to **stdout**. Errors go to **stderr** with a non-zero exit code.

### Notes

- Devices must be **discoverable / advertising** during the scan window.
- Classification uses BlueZ **UUIDs** and **Class**; unusual firmware stacks might omit these until paired.
- Ensure Wi‑Fi/Bluetooth coexistence is acceptable on your Pi (metal cases and poor antenna placement reduce range).
