#!/usr/bin/env bash
# Deploy the music-card-player source tree to a Raspberry Pi via scp.
#
# Copies the CONTENTS of the music-card-player/ folder into the remote
# directory, so Makefile and src/ end up directly inside $DEST.
#
# Usage:
#   ./deploy.sh [user@host] [remote_path]
#
# Defaults:
#   user@host   = pi@raspberrypi.local
#   remote_path = ~/music-card-player

REMOTE="${1:-pi@raspberrypi.local}"
DEST="${2:-~/music-card-player}"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SOURCE="$SCRIPT_DIR/music-card-player"

if [ ! -d "$SOURCE" ]; then
    echo "Source folder not found: $SOURCE" >&2
    exit 1
fi

echo "══════════════════════════════════════════════"
echo "  Source : $SOURCE"
echo "  Remote : $REMOTE:$DEST"
echo "══════════════════════════════════════════════"
echo ""

# Ensure remote directory exists
echo "Creating remote directory..."
ssh "$REMOTE" "mkdir -p $DEST"

# Copy contents (trailing /. means "everything inside this folder")
echo "Copying files..."
scp -rv "$SOURCE/." "$REMOTE:$DEST"

if [ $? -eq 0 ]; then
    echo ""
    echo "Deploy complete."
else
    echo ""
    echo "Deploy failed (exit code $?)." >&2
    exit 1
fi
