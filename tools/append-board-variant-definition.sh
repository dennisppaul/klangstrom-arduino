#!/bin/zsh

# append board definition e.g './append-board-variant-definition.sh ../variants/KLST_PANDA/KLST_PANDA-boards.txt'

source stm32duino.config

check_line_in_file() {
    local FILE="$1"
    local LINE="$2"

    if [ ! -f "$FILE" ]; then
        echo "Error: File $FILE does not exist."
        return 1
    fi

    if grep -Fxq "$LINE" "$FILE"; then
        return 0
    else
        return 1
    fi
}

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <board_definition_file>"
    exit 1
fi

BOARD_DEFINITION_FILE="$1"
if [ ! -f "$BOARD_DEFINITION_FILE" ]; then
    echo "Error: File $BOARD_DEFINITION_FILE does not exist."
    exit 1
fi

LINE_TO_FIND=$(tail -n 1 "$BOARD_DEFINITION_FILE")

FILE_TO_CHECK=$ARDUINO_STM32DUINO_BOARD_FILE

if check_line_in_file "$FILE_TO_CHECK" "$LINE_TO_FIND"; then
    echo "board definition is present in '$FILE_TO_CHECK'."
    echo "nothing to append."
else
    echo "ok, board definition is NOT present in '$FILE_TO_CHECK'."
    echo -n "appending board defintion '$BOARD_DEFINITION_FILE' now ... "
    cat "$BOARD_DEFINITION_FILE" >> "$FILE_TO_CHECK"
    echo "done."
fi
