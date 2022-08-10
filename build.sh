#! /bin/zsh

build_archive() {
    tar -zcpf $1.tar $1
    ls -l $1.tar
    sha256sum $1.tar
}

build_archive klangstrom-arduino-stm32-0.0.3
build_archive klangstrom-arduino-desktop-0.0.3
