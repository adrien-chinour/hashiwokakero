AR=ar
CC=gcc
CFLAGS= -Wall -std=c99
ARFLAGS=rcs
OBJETS=src/game.o src/node.o src/interface.o src/generate.o
EXEC =hashi_text

all: $(OBJETS) libhashi.a hashi_text

src/%.o : src/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

libhashi.a: $(OBJETS)
	$(AR) $(ARFLAGS) $@ $^

hashi_text : src/hashi_text.c libhashi.a
	$(CC) -g -o $@ $(CFLAGS) $< -L. -lhashi

# make test : créer les executables des tests sur les fonctions de
# node.c et game.c puis les exécutes

test :test1 test2 test3 test4 test5 test6 test7 test9
	@ ./test1 ; ./test2 ; ./test3 ; ./test4 ; ./test5 ; ./test6 ; ./test7 ;./test9
	@ echo Tests terminés.

test%: tests/test_game%.c tests/test_toolbox.c libhashi.a
	@ $(CC) -g -o $@ $(CFLAGS) $< tests/test_toolbox.c -L. -lhashi

# make clean : nettoie les fichiers créés par le Makefile

clean:
	@ rm -rf src/*.o src/*~ test1 test2 test3 test4 test5 test6 test7 test9 tests/*~ include/*~ *.a $(EXEC)
	@ echo Nettoyage terminé.
