CC=gcc
CFLAGS=-std=c99 -O3 -Wall -I/usr/include/SDL2
LDFLAGS=-rdynamic -lm -lSDL2 -lSDL2_ttf -lSDL2_image

ALL: main

main: hashi_gui.o model.o file.o game.o node.o

clean:
	rm -f *~ *.o demo
