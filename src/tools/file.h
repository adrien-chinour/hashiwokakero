#ifndef _GENERATE_H_
#define _GENERATE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"

game generate_game(int game, int nb_max_bridges, int nb_dir, int nb_nodes);
bool check_file(char *file);
game translate_game(char * fileopen);
game translate_save(char * fileopen);
void write_save(game g, char * filesave);

#endif // _GENERATE_H_
