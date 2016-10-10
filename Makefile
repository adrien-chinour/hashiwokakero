OBJETS = game.o node.o
CFLAGS = -Wall -std=c99
LDFLAGS = -L. -lhashi
EXEC = hashi_text
CC = gcc

all: hashi_text libhashi.a

hashi_text: hashi_text.c libhashi.a
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) 

libhashi.a: $(OBJETS)
	ar rcs $@ $^

clean:
	rm -f *.a *~ $(EXEC)
