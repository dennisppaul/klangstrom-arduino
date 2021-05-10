#! /bin/zsh

PATH_TO_MODULES=../klangstrom/klangstrom/platforms/klangstrom-arduino
MY_PATH=`pwd`

copyPlatfrom() {
    echo "### copying platform             : $1"
    DST_DIR=$MY_PATH/$1
    SRC_DIR=$PATH_TO_MODULES/$1

    if [ -d $DST_DIR ]; then
        echo "###     removing existing folder : $DST_DIR"
        rm -r $DST_DIR
    fi

    echo "###     copying folder           : $SRC_DIR"
    cp -r $SRC_DIR $MY_PATH
}

echo 
echo "###"
echo "### copying platform package from development repository to arduino library distribution repository"
echo "###"

copyPlatfrom shared
copyPlatfrom stm32
copyPlatfrom desktop
#copyModul vcvrack

echo "###"
echo 
