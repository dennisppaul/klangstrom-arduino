#!/bin/bash

#/usr/bin/osascript -e "tell application \"Arduino\" to display dialog \"$1\""
exec $1 &
