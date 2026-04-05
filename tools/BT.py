#!/usr/bin/env python3
"""
Discover nearby Bluetooth audio-class devices using BlueZ via the system D-Bus.

Runs a timed discovery scan (default 10s), then lists human-readable device
names (Alias, falling back to Name) for devices that advertise audio profiles
(A2DP, HSP, HFP) or Audio/Video rendering class-of-device.
"""

from __future__ import annotations

import sys
import time
from typing import Any

# System D-Bus is provided by python3-dbus on Raspberry Pi OS.
try:
    import dbus
except ImportError:  # pragma: no cover
    print(
        "Missing dependency: install with  sudo apt install python3-dbus",
        file=sys.stderr,
    )
    sys.exit(1)

BUS_NAME = "org.bluez"
ROOT_PATH = "/"
OBJECT_MANAGER_IFACE = "org.freedesktop.DBus.ObjectManager"
ADAPTER_IFACE = "org.bluez.Adapter1"
DEVICE_IFACE = "org.bluez.Device1"

# Base UUID fragments (no dashes) for common audio services / profiles.
_AUDIO_UUID_BASES = frozenset(
    {
        "0000110a",  # Audio Source
        "0000110b",  # Audio Sink (A2DP)
        "0000110d",  # Advanced Audio Distribution Profile (A2DP) — some stacks
        "0000110e",  # Handsfree Audio Gateway / HFP related
        "00001108",  # Headset profile
        "0000111e",  # Handsfree
    }
)

# Bluetooth major device class (5 bits): Audio/Video (e.g. headset, speaker).
_BT_MAJOR_AUDIO_VIDEO = 0x04


def _normalize_class(value: Any) -> int | None:
    if value is None:
        return None
    try:
        return int(value)
    except (TypeError, ValueError):
        return None


def _major_device_class(class_of_device: int) -> int:
    """Extract major device class from a 24-bit Class of Device (CoD) value."""
    return (class_of_device >> 8) & 0x1F


def _uuid_base_key(uuid: Any) -> str:
    s = str(uuid).lower().replace("-", "")
    return s[:8] if len(s) >= 8 else s


def _device_has_audio_profile(uuids: Any) -> bool:
    if not uuids:
        return False
    for u in uuids:
        base = _uuid_base_key(u)
        if any(base.startswith(p) for p in _AUDIO_UUID_BASES):
            return True
        if base in _AUDIO_UUID_BASES:
            return True
    return False


def _device_is_audio_class(class_of_device: int | None) -> bool:
    if class_of_device is None:
        return False
    return _major_device_class(class_of_device) == _BT_MAJOR_AUDIO_VIDEO


def _is_audio_device(props: dict[str, Any]) -> bool:
    raw_uuids = props.get("UUIDs")
    raw_class = props.get("Class")
    cls = _normalize_class(raw_class)
    return _device_has_audio_profile(raw_uuids) or _device_is_audio_class(cls)


def _props_dict(iface_props: Any) -> dict[str, Any]:
    """Normalize BlueZ property mappings to str keys (dbus-python quirk)."""
    return {str(k): v for k, v in dict(iface_props).items()}


def _display_name(props: dict[str, Any]) -> str:
    alias = props.get("Alias")
    name = props.get("Name")
    for candidate in (alias, name):
        if candidate:
            return str(candidate).strip() or "(unknown)"
    return "(unknown)"


def _get_managed_objects(bus: dbus.SystemBus) -> dict:
    obj = bus.get_object(BUS_NAME, ROOT_PATH)
    om = dbus.Interface(obj, OBJECT_MANAGER_IFACE)
    return om.GetManagedObjects()


def _find_adapter_path(objects: dict) -> str | None:
    for path, ifaces in objects.items():
        if ADAPTER_IFACE in ifaces:
            return str(path)
    return None


def discover_nearby_audio_devices(scan_seconds: float = 10.0) -> list[str]:
    """
    Run BlueZ discovery for ``scan_seconds``, then return sorted unique names
    of devices classified as audio (see module docstring).
    """
    bus = dbus.SystemBus()
    objects = _get_managed_objects(bus)
    adapter_path = _find_adapter_path(objects)
    if not adapter_path:
        raise RuntimeError("No BlueZ adapter found (is Bluetooth enabled?)")

    adapter = dbus.Interface(bus.get_object(BUS_NAME, adapter_path), ADAPTER_IFACE)
    adapter.StartDiscovery()
    try:
        time.sleep(scan_seconds)
    finally:
        try:
            adapter.StopDiscovery()
        except dbus.exceptions.DBusException:
            # Adapter may already have stopped discovery in edge cases.
            pass

    objects = _get_managed_objects(bus)
    seen: set[str] = set()
    names: list[str] = []
    for path, ifaces in objects.items():
        if DEVICE_IFACE not in ifaces:
            continue
        props = _props_dict(ifaces[DEVICE_IFACE])
        if not _is_audio_device(props):
            continue
        label = _display_name(props)
        if label not in seen:
            seen.add(label)
            names.append(label)
    names.sort(key=str.lower)
    return names


def main() -> int:
    try:
        devices = discover_nearby_audio_devices(10.0)
    except dbus.exceptions.DBusException as e:
        print(f"D-Bus error (is the bluetooth service running?): {e}", file=sys.stderr)
        return 1
    except RuntimeError as e:
        print(str(e), file=sys.stderr)
        return 1

    for n in devices:
        print(n)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
