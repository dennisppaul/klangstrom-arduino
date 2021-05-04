#! /bin/zsh

PATH_TO_MODULES=../klangstrom/klangstrom/platforms/klangstrom-arduino/klangstrom
MY_PATH=`pwd`

copyModul() {
    echo "### copying module          : $1"
    DST_DIR=$MY_PATH/$1
    SRC_DIR=$PATH_TO_MODULES/$1

    if [ -d $DST_DIR ]; then
        echo "    removing existing folder: $DST_DIR"
        rm -rf $DST_DIR
    fi

    echo "    copying folder          : $SRC_DIR"
    cp -r $SRC_DIR $MY_PATH
}

echo "### copying modules from development repository to arduino library distribution repository"
echo 

copyModul shared
copyModul stm32
#copyModul vcvrack
#copyModul desktop
