#!/bin/zsh

git fetch klib
git subtree pull --prefix=libraries klib main --squash
