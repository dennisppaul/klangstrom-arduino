#! /bin/zsh

build_archive() {
	M_EXTENSION=tar.bz2
    tar -cpjSf $1.$M_EXTENSION $1
    ls -l $1.$M_EXTENSION
    sha256sum $1.$M_EXTENSION
}

build_archive klangstrom-arduino-stm32-0.0.3
build_archive klangstrom-arduino-desktop-0.0.3
