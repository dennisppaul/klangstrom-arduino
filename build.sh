#! /bin/zsh

PATH_TO_ARCHIVES=../

print_line() {
    echo "--------------------------------------------------------------------------------------------------------"
}

build_archive() {
    M_PACKAGE_NAME=$1
    M_KLST_LIB_PATH=klangstrom/klangstrom/platforms/klangstrom-arduino
    M_VERSION=$2
    M_EXTENSION=tar.bz2
    M_ARCHIVE_NAME=klangstrom-arduino-$M_PACKAGE_NAME-$M_VERSION

    echo
    print_line
    echo $M_ARCHIVE_NAME
    print_line
#     tar -cpjSf $M_ARCHIVE_NAME.$M_EXTENSION -C $M_KLST_LIB_PATH/$M_PACKAGE_NAME .
    cp $PATH_TO_ARCHIVES/$M_ARCHIVE_NAME.$M_EXTENSION .
    echo -n "checksum: SHA-256:"
    sha256sum $M_ARCHIVE_NAME.$M_EXTENSION | awk '{print $1}'
    echo -n "size: "
    ls -l $M_ARCHIVE_NAME.$M_EXTENSION | awk '{print $5}'

    print_line
    echo
}

build_archive desktop 0.0.3
build_archive stm32 0.0.3
