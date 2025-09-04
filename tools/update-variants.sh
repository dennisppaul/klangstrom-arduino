#!/bin/zsh

source stm32duino.config

update_variant() {
    echo "updating" $1
    VARIANT_PATH=../variants/$1
    BOARD_FILE=$1-boards.txt
    ./link_variant_files.sh $VARIANT_PATH/variant "$ARDUINO_STM32DUINO_PATH"
    ./append-board-variant-definition.sh $VARIANT_PATH/$BOARD_FILE
}

echo "removing all symlinks from '$ARDUINO_STM32DUINO_PATH'"
./remove_linked_variant_files.sh "$ARDUINO_STM32DUINO_PATH"

update_variant KLST_PANDA
update_variant KLST_CATERPILLAR
