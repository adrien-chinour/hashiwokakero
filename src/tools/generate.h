#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../solver/solver.h"

node * generate_nodes(int nb_max_bridges, int nb_dir);

bool test_game(game g, int nb_max_bridges, int nb_dir);

game random_game(int nb_max_bridges, int nb_dir);
