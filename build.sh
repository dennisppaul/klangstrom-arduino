#! /bin/zsh

ROOT_DIR=$(dirname "$(readlink -f "$0")")
source $ROOT_DIR/../build.config

PATH_TO_ARCHIVES=../

print_line() {
    echo "--------------------------------------------------------------------------------------------------------"
}

build_archive() {
    M_PACKAGE_NAME=$1
    M_KLST_LIB_PATH=klangstrom/klangstrom/platforms/klangstrom-arduino
    M_VERSION=$2
    M_EXTENSION=tar.bz2
    M_PROJECT_PREFIX=klangstrom-arduino
    M_ARCHIVE_NAME=$M_PROJECT_PREFIX-$M_PACKAGE_NAME-$M_VERSION
    M_JSON_SUFFIX=_index.json
    M_JSON_NAME=package_$M_PROJECT_PREFIX-$M_PACKAGE_NAME$M_JSON_SUFFIX

    echo
    print_line
    echo $M_ARCHIVE_NAME
    print_line
#     tar -cpjSf $M_ARCHIVE_NAME.$M_EXTENSION -C $M_KLST_LIB_PATH/$M_PACKAGE_NAME .
#     cp $PATH_TO_ARCHIVES/$M_ARCHIVE_NAME.$M_EXTENSION .

    CHECKSUM=$(sha256sum $M_ARCHIVE_NAME.$M_EXTENSION | awk '{print $1}')
    echo "checksum: SHA-256:" $CHECKSUM

    SIZE=$(ls -l $M_ARCHIVE_NAME.$M_EXTENSION | awk '{print $5}')
    echo "size:" $SIZE

    cat $M_JSON_NAME.template | sed -E "s/\{\{SIZE\}\}/${SIZE}/;s/\{\{CHECKSUM\}\}/${CHECKSUM}/"\ > $M_JSON_NAME

    print_line
    echo
}

build_archive desktop $KLST_ARDUINO_VERSION
build_archive stm32 $KLST_ARDUINO_VERSION
