# Deploy the music-card-player source tree to a Raspberry Pi via scp.
#
# Copies the CONTENTS of the music-card-player/ folder into the remote
# directory, so Makefile and src/ end up directly inside $Dest.
#
# Usage:
#   .\deploy.ps1 [user@host] [remote_path]
#
# Defaults:
#   user@host   = pi@raspberrypi.local
#   remote_path = ~/music-card-player

param(
    [string]$Remote = "will@truelinelaserspi5.local",
    [string]$Dest   = "~/music-card-player"
)

$Source = Join-Path $PSScriptRoot "music-card-player"

if (-not (Test-Path $Source)) {
    Write-Error "Source folder not found: $Source"
    exit 1
}

Write-Host ""
Write-Host "==================================================="
Write-Host "  Source : $Source"
Write-Host "  Remote : ${Remote}:${Dest}"
Write-Host "==================================================="
Write-Host ""

# Ensure remote directory exists
Write-Host "Creating remote directory..."
ssh $Remote "mkdir -p $Dest"

# Copy contents (trailing /. means "everything inside this folder")
Write-Host "Copying files..."
Write-Host ""
scp -r "$Source/." "${Remote}:${Dest}"

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "Deploy complete." -ForegroundColor Green
} else {
    Write-Host ""
    Write-Error "Deploy failed (exit code $LASTEXITCODE)."
    exit 1
}
