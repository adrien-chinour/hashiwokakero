#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../tools/file.h"

int get_nb_neighbours(game g, int num);

void bridges_in_all_directions(game g);

void simple_bridges(game g);

int * calcul_bridges_neighbours(int node_num, game g);

void apply_bridges_neighbours(game g, int node_num, int * bridges);

void auto_play(game g, int node_num, bool change);

bool solver_r(game g,int node_num,int dir,bool * go);
