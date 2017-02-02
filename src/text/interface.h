#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/generate.h"

typedef struct s_map {
  int size;
  int** matrix;
} *hashiMap;


void starting(void);

void draw_hashiMap(cgame g, hashiMap m);

void print_game(cgame g);

void add_bridge(game g, hashiMap m);

void del_bridge(game g, hashiMap m);

hashiMap create_hashiMap(cgame g);

void delete_hashiMap(hashiMap m);

void test_game_over(game g);

void reset_game(game g, hashiMap m);

game game_select();

void save_game(game g);

#endif // _INTERFACE_H_
