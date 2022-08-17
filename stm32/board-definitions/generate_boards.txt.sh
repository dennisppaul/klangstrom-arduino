#! /bin/zsh

cat \
general.boards.txt \
KLST_SHEEP.boards.txt \
KLST_TINY.boards.txt \
KLST_CORE.boards.txt \
> boards.txt
mv boards.txt ../boards.txt
