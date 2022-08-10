#! /bin/zsh

PATH_TO_MODULES=../klangstrom/klangstrom/platforms/klangstrom-arduino
MY_PATH=`pwd`

copyPlatfrom() {
    echo "### copying platform             : $1"
    DST_DIR=$MY_PATH/$1
    SRC_DIR=$PATH_TO_MODULES/$1

    if [ -d $DST_DIR ]; then
        echo "###     removing existing folder : $DST_DIR"
        rm -rf $DST_DIR
    fi

    echo "###     copying folder           : $SRC_DIR"
    cp -r $SRC_DIR $MY_PATH
}

echo "###"
echo "### copying platform package from development repository to arduino library distribution repository"
echo "###"

# copyPlatfrom shared
copyPlatfrom stm32
copyPlatfrom desktop
#copyModul vcvrack

echo "###"

source ../build.config

# copy examples to doc
EXAMPLE_CODE_DOC_FOLDER=./docs/_includes/code/
mkdir -p $EXAMPLE_CODE_DOC_FOLDER
for folder in $KLST_ARDUINO_LIBRARIES
do
	echo "### copying examples from '$folder'"
#     find $PATH_TO_MODULES/libraries/$folder/examples/ -name '*.ino' -exec echo {} \;
    find $PATH_TO_MODULES/libraries/$folder/examples/ -name 'Example*.ino' -exec cp {} $EXAMPLE_CODE_DOC_FOLDER \;
done

echo "###"
