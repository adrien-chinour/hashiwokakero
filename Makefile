AR=ar
CC=gcc
CFLAGS= -Wall -std=c99
ARFLAGS=rcs
SRCDIR= ./src/
OBJETS=src/game.o src/node.o
EXEC =hashi_text
all: libhashi.a hashi_text

libhashi.a: $(OBJETS)
	$(AR) $(ARFLAGS) $@ $^

hashi_text:./src/hashi_text.c libhashi.a
	$(CC) -o $@ $(CFLAGS) $< -L. -lhashi
clean:
	rm -rf *.a *~ $(EXEC)
