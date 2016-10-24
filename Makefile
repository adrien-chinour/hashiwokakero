AR=ar
CC=gcc
CFLAGS= -Wall -std=c99
ARFLAGS=rcs
SRCDIR= ./src/
OBJETS=src/game.o src/node.o
EXEC =hashi_text
all: src/game.o src/node.o libhashi.a hashi_text

src/game.o : src/game.c
	$(CC) -c $(CFLAGS) $^ -o $@

src/node.o : src/node.c
	$(CC) -c $(CFLAGS) $^ -o $@

libhashi.a: $(OBJETS)
	$(AR) $(ARFLAGS) $@ $^

hashi_text : src/hashi_text.c libhashi.a
	$(CC) -o $@ $(CFLAGS) $< -L. -lhashi

test : tests/test_game1.c tests/test_toolbox.c libhashi.a
	$(CC) -o $@ $(CFLAGS) $< tests/test_toolbox.c -L. -lhashi

clean:
	rm -rf src/*~ test tests/*~ include/*~ *.a $(EXEC)

