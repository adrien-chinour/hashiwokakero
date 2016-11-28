#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "node.h"
#include "game.h"
#include "generate.h"

void starting(void);

void print_matrix(cgame g);

void print_game(cgame g);

void add_bridge(game g);

void del_bridge(game g);

void create_matrix(cgame g);

void test_game_over(game g);

void reset_game(game g);

game game_select();

#endif // _INTERFACE_H_