#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../include/game.h"
#include "test_toolbox.h"

/* Defalult configuration
   2     2     3

   5     1

   3           2
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
      r = test_interval_int(0,EXAMPLE_NB_NODE-1, num, " Pb game_node_number") && r;
      node n = game_node(g, num);
      r = test_equality_int(vals[i][2], get_required_degree(n), " Pb with degrees") && r;
    }
    r = test_equality_int(-1, game_get_node_number (g, 1, 0), " Pb game_node_number") && r;
    r = test_equality_int(-1, game_get_node_number (g, 2, 1), " Pb game_node_number") && r;
    delete_game(g);
    return r;
}

bool test_can_add_bridge_dir() {
    game g = default_game();
    bool r = true;
    int num = game_get_node_number (g, 0, 0);
    r = test_equality_bool(true, can_add_bridge_dir(g, num, NORTH), " can add NORTH") && r;
    r = test_equality_bool(true, can_add_bridge_dir(g, num, EAST), " can add EAST") && r;
    r = test_equality_bool(false, can_add_bridge_dir(g, num, WEST), " can add WEST") && r;
    r = test_equality_bool(false, can_add_bridge_dir(g, num, SOUTH), " can add SOUTH") && r;
    delete_game(g);
    return r;
}



bool test_get_neighbour() {
    game g = default_game();
    bool r = true;
    r = test_equality_int(5, get_neighbour_dir(g,0,EAST), " pb neighbour 0 EAST") && r;
    r = test_equality_int(4, get_neighbour_dir(g,2,EAST), " pb neighbour 2 EAST") && r;
    r = test_equality_int(0, get_neighbour_dir(g,1,SOUTH), " pb neighbour 1 SOUTH") && r;
    r = test_equality_int(2, get_neighbour_dir(g,1,NORTH), " pb neighbour 1 NORTH") && r;
    delete_game(g);
    return r;
}



int main (int argc, char *argv[])
{
   printf("test1:\n");
   bool result= true;
   result = test_get_neighbour() && result;
   result ? printf(" test_get_neighbour ok \n") : printf(" test_get_neighbour not ok \n");
   result = test_get_node_number() && result;
   result ? printf(" test_node_number ok \n") : printf(" test_node_number not ok \n");
   result = test_can_add_bridge_dir() && result;
   result ? printf(" test_can_add_bridge_dir ok \n") : printf(" test_can_add_bridge_dir not ok \n");
   if (result){
      fprintf(stdout,"test1 success\n");
      return EXIT_SUCCESS;
   }

   else{
      fprintf(stderr,"test1 failure: get_neighbour+can_add_bridge+get_node_num\n");
      return EXIT_FAILURE;
   }

}
