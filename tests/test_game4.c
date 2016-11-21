#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../include/game.h"
#include "test_toolbox.h"

/* Defalult configuration
   1     2     .

   3     7     2

   2     4     1
*/
#define EXAMPLE_NB_NODE 7
#define NB_BRIDGES_MAX 2
#define NB_DIR 4
int vals[EXAMPLE_NB_NODE][3] = {{0,0,3},{0,1,5},{0,2,2},{1,1,1},{1,2,2},{2,0,2},{2,2,3}};

static game default_game (){
    node nodes[EXAMPLE_NB_NODE];
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
    game g = new_game(EXAMPLE_NB_NODE, nodes,NB_BRIDGES_MAX,NB_DIR);
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      delete_node(nodes[i]);
    return g;
}




bool test_get_node_number() {
    game g = default_game();
    bool r = true;

    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++) {
      int num = game_get_node_number (g, vals[i][0], vals[i][1]);
      r = r && test_interval_int(0,EXAMPLE_NB_NODE-1, num, "Pb game_node_number");
      node n = game_node(g, num);
      r = r && test_equality_int(vals[i][2], get_required_degree(n), "Pb with degrees");
    }
    r = r && test_equality_int(3, game_get_node_number (g, 1, 0), "Pb game_node_number");
    r = r && test_equality_int(7, game_get_node_number (g, 2, 1), "Pb game_node_number");
    delete_game(g);
    return r;
}

bool test_can_add_bridge_dir() {
    game g = default_game();
    bool r = true;
    int num = game_get_node_number (g, 0, 0);
    r = r && test_equality_bool(true, can_add_bridge_dir(g, num, NORTH), "can add NORTH");
    r = r && test_equality_bool(true, can_add_bridge_dir(g, num, EAST), "can add EAST");
    r = r && test_equality_bool(false, can_add_bridge_dir(g, num, WEST), "can add WEST");
    r = r && test_equality_bool(false, can_add_bridge_dir(g, num, SOUTH), "can add SOUTH");
    delete_game(g);
    return r;
}



bool test_get_neighbour() {
    game g = default_game();
    bool r = true;
    r = r && test_equality_int(3, get_neighbour_dir(g,0,EAST), "pb neighbour 0 EAST");
    r = r && test_equality_int(5, get_neighbour_dir(g,2,EAST), "pb neighbour 2 EAST");
    r = r && test_equality_int(0, get_neighbour_dir(g,1,SOUTH), "pb neighbour 1 SOUTH");
    r = r && test_equality_int(2, get_neighbour_dir(g,1,NORTH), "pb neighbour 1 NORTH");
    delete_game(g);
    return r;
}



int main (int argc, char *argv[])
{
    bool result= true;
    result = result && test_get_neighbour();
    result = result && test_get_node_number();
    result = result && test_can_add_bridge_dir();
    if (result){
       fprintf(stdout,"test4 success\n");
       return EXIT_SUCCESS;
    }

    else
        return EXIT_FAILURE;
}
