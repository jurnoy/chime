#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra"
LIBS="`pkg-config --libs raylib` -lGL -lm -lpthread -ldl -lrt -lX11"

clang $CFLAGS -o chime src/main.c $LIBS src/screen_title.c src/screen_game.c
