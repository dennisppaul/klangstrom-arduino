#!/usr/bin/env bash
set -euo pipefail

# Cross-platform linker for Arduino libraries
# - Works on macOS, Linux, Windows (Git Bash/MSYS/Cygwin/WSL)
# - Creates symlinks on Unix; on Windows tries symlink first, then junction
#
# Usage:
#   ./link_arduino_libs.sh --list klangstrom-libraries.txt --src ../libraries
# Options:
#   --dest <path>   Override Arduino libraries destination
#   --force         Replace existing entries that point elsewhere
#   --dry-run       Show actions without changing anything
#
# Env override:
#   ARDUINO_LIB_DIR=/custom/path

LIST=""
SRC=""
DEST=""
FORCE=0
DRYRUN=0

err(){ printf "ERROR: %s\n" "$*" >&2; exit 1; }
note(){ printf "%s\n" "$*"; }

is_windows() {
  case "${OS:-}" in
    Windows_NT) return 0 ;;
  esac
  case "$(uname -s 2>/dev/null || echo unknown)" in
    MINGW*|MSYS*|CYGWIN*|Windows) return 0 ;;
    *) return 1 ;;
  esac
}

win_path() {
  # Convert POSIX → Windows path if cygpath exists; otherwise best effort
  if command -v cygpath >/dev/null 2>&1; then
    cygpath -w "$1"
  else
    # WSL: use wslpath if available
    if command -v wslpath >/dev/null 2>&1; then
      wslpath -w "$1"
    else
      # Fallback: just echo
      printf "%s" "$1"
    fi
  fi
}

default_arduino_lib_dir() {
  if [ -n "${ARDUINO_LIB_DIR:-}" ]; then
    printf "%s\n" "$ARDUINO_LIB_DIR"
    return
  fi

  if is_windows; then
    local docs
    # Prefer %USERPROFILE%\Documents (works with OneDrive redirection in most setups)
    docs="${USERPROFILE:-}/Documents"
    [ -z "$docs" ] && docs="${HOME:-$PWD}/Documents"
    printf "%s\n" "$docs/Arduino/libraries"
    return
  fi

  case "$(uname -s)" in
    Darwin) printf "%s\n" "$HOME/Documents/Arduino/libraries" ;;
    Linux)  printf "%s\n" "$HOME/Arduino/libraries" ;;
    *)      printf "%s\n" "$HOME/Arduino/libraries" ;;
  esac
}

parse_args() {
  while [ $# -gt 0 ]; do
    case "$1" in
      --list) LIST="${2:-}"; shift 2 ;;
      --src)  SRC="${2:-}"; shift 2 ;;
      --dest) DEST="${2:-}"; shift 2 ;;
      --force) FORCE=1; shift ;;
      --dry-run) DRYRUN=1; shift ;;
      -h|--help)
        sed -n '1,80p' "$0" | sed 's/^# \{0,1\}//'
        exit 0 ;;
      *)
        err "Unknown arg: $1"
        ;;
    esac
  done

  [ -n "$LIST" ] || err "--list is required"
  [ -n "$SRC" ]  || err "--src is required"
  [ -f "$LIST" ] || err "List file not found: $LIST"
  [ -d "$SRC" ]  || err "Source root not found: $SRC"

  if [ -z "$DEST" ]; then
    DEST="$(default_arduino_lib_dir)"
  fi
}

ensure_dir() {
  [ -d "$1" ] || { [ "$DRYRUN" -eq 1 ] && note "[dry-run] mkdir -p $1" || mkdir -p "$1"; }
}

same_target() {
  # Try to detect if dst already points to src (works reliably for symlinks)
  local dst="$1" src="$2"
  if [ -L "$dst" ]; then
    local target
    target="$(readlink "$dst")" || return 1
    # Normalize
    local abs_dst abs_src
    abs_dst="$(cd "$(dirname "$dst")" && cd "$(dirname "$target")" && pwd)/$(basename "$target")"
    abs_src="$(cd "$src" && pwd)"
    [ "$abs_dst" = "$abs_src" ]
    return
  fi
  # For Windows junctions or regular dirs we can’t be certain; return false
  return 1
}

remove_path() {
  local p="$1"
  if [ -L "$p" ]; then
    [ "$DRYRUN" -eq 1 ] && note "[dry-run] rm $p" || rm "$p"
    return
  fi
  if is_windows; then
    # Junctions are directories; rmdir handles them without touching contents
    [ "$DRYRUN" -eq 1 ] && note "[dry-run] rmdir \"$p\"" || rmdir "$p" 2>/dev/null || rm -rf "$p"
    return
  fi
  [ -d "$p" ] && { [ "$DRYRUN" -eq 1 ] && note "[dry-run] rm -rf $p" || rm -rf "$p"; } || true
}

link_unix() {
  local src="$1" dst="$2"
  [ "$DRYRUN" -eq 1 ] && { note "[dry-run] ln -s \"$src\" \"$dst\""; return; }
  ln -s "$src" "$dst"
}

link_windows() {
  local src="$1" dst="$2"
  local wsrc wdst
  wsrc="$(win_path "$src")"
  wdst="$(win_path "$dst")"

  if command -v powershell.exe >/dev/null 2>&1; then
    # Try symbolic link first
    if [ "$DRYRUN" -eq 1 ]; then
      note "[dry-run] PowerShell: New-Item -ItemType SymbolicLink -Path \"$wdst\" -Target \"$wsrc\" || Junction"
      return
    fi
    powershell.exe -NoProfile -Command "try { New-Item -ItemType SymbolicLink -Path \"$wdst\" -Target \"$wsrc\" -ErrorAction Stop | Out-Null } catch { \$e=\$_.Exception.Message; try { New-Item -ItemType Junction -Path \"$wdst\" -Target \"$wsrc\" -ErrorAction Stop | Out-Null } catch { Write-Error \"Link failed: \$e / \$($_.Exception.Message)\"; exit 1 } }" || return 1
    return 0
  fi

  # Fallback to cmd mklink /J
  if command -v cmd.exe >/dev/null 2>&1; then
    if [ "$DRYRUN" -eq 1 ]; then
      note "[dry-run] cmd /c mklink /J \"$wdst\" \"$wsrc\""
      return
    fi
    cmd.exe /c "mklink /J \"$wdst\" \"$wsrc\"" >/dev/null || return 1
    return 0
  fi

  err "No powershell.exe or cmd.exe found to create links on Windows"
}

create_link() {
  local src="$1" dst="$2"
  if is_windows; then
    link_windows "$src" "$dst"
  else
    link_unix "$src" "$dst"
  fi
}

main() {
  parse_args "$@"

  LIST="$(cd "$(dirname "$LIST")" && pwd)/$(basename "$LIST")"
  SRC="$(cd "$SRC" && pwd)"
  DEST="${DEST/#\~/$HOME}"
  DEST="$(printf "%s" "$DEST" | sed 's#//#/#g')"

  ensure_dir "$DEST"

  note "Source root : $SRC"
  note "Dest root   : $DEST"
  note "List        : $LIST"
  note

  local errors=0
  while IFS= read -r line || [ -n "$line" ]; do
    # Trim comments/whitespace
    line="${line%%#*}"
    line="$(printf "%s" "$line" | sed 's/^[[:space:]]\+//; s/[[:space:]]\+$//')"
    [ -z "$line" ] && continue

    local src="$SRC/$line"
    local dst="$DEST/$line"

    if [ ! -d "$src" ]; then
      note "[skip] missing source: $src"
      continue
    fi

    if [ -e "$dst" ] || [ -L "$dst" ]; then
      if same_target "$dst" "$src"; then
        note "[ok] already linked: $dst"
        continue
      fi
      if [ "$FORCE" -eq 1 ]; then
        note "[replace] removing: $dst"
        remove_path "$dst" || { note "  failed to remove: $dst"; errors=$((errors+1)); continue; }
      else
        note "[skip] exists (different target): $dst  (use --force to replace)"
        continue
      fi
    fi

    ensure_dir "$(dirname "$dst")"
    note "[link] $dst -> $src"
    if ! create_link "$src" "$dst"; then
      note "  ERROR linking $dst"
      errors=$((errors+1))
    fi
  done < "$LIST"

  [ "$errors" -eq 0 ] || exit 1
}

main "$@"