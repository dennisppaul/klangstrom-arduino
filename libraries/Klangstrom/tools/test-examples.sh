#!/usr/bin/env bash
set -euo pipefail

EXAMPLES_DIR="../examples"
PLATFORMS=(
  "STMicroelectronics:stm32:KLST_PANDA"
  "umfeld-arduino:umfeld:KLST_EMU"
)

RESULTS_FILE="compile-results.csv"
: > "$RESULTS_FILE"   # empty the file at start

# write CSV header
{
  printf "Example"
  for PLATFORM in "${PLATFORMS[@]}"; do
    printf ",%s" "$PLATFORM"
  done
  printf "\n"
} >> "$RESULTS_FILE"

# collect example dirs that actually contain a .ino
mapfile -t EXAMPLES < <(find "$EXAMPLES_DIR" -mindepth 2 -maxdepth 2 -type d | while read -r d; do
  find "$d" -maxdepth 1 -name "*.ino" | grep -q . && echo "$d"
done)

# progress bar helper
progress() {
  local current="$1" total="$2" label="$3"
  local width=40
  local percent=$(( total == 0 ? 100 : current * 100 / total ))
  local filled=$(( percent * width / 100 ))
  local empty=$(( width - filled ))

  # \r = move to beginning of line, \033[K = clear to end of line
  printf "\r\033[K["
  printf "%0.s#" $(seq 1 "$filled")
  printf "%0.s-" $(seq 1 "$empty")
  printf "] %3d%% (%d/%d) %s" "$percent" "$current" "$total" "$label"
}

total_tasks=$(( ${#EXAMPLES[@]} * ${#PLATFORMS[@]} ))
done_tasks=0

for EXAMPLE in "${EXAMPLES[@]}"; do
  ROW="$(basename "$EXAMPLE")"
  for PLATFORM in "${PLATFORMS[@]}"; do
    progress "$done_tasks" "$total_tasks" "Compiling: $(basename "$EXAMPLE") on $PLATFORM"
#     if arduino-cli compile -b "$PLATFORM" "$EXAMPLE" >/dev/null 2>&1; then
    if arduino-cli compile -b "$PLATFORM" "$EXAMPLE" >/dev/null 2>&1; then
      ROW="$ROW,✅ Success"
    else
      ROW="$ROW,❌ Fail"
    fi
    done_tasks=$((done_tasks + 1))
  done
  echo "$ROW" >> "$RESULTS_FILE"
done

# final 100% line + newline
progress "$total_tasks" "$total_tasks" "Done"
echo
echo "CSV table written to $RESULTS_FILE"