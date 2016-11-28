#ifndef _GENERATE_H_
#define _GENERATE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "node.h"
#include "game.h"

game generate_game(int game, int nb_max_bridges, int nb_dir);

#endif // _GENERATE_H_