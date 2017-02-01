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
#define EXAMPLE_NB_NODE 8
#define NB_BRIDGES_MAX 2
#define NB_DIR 4
int vals[EXAMPLE_NB_NODE][3] = {{0,0,2},{0,1,3},{0,2,1},{1,0,4},{1,1,7},{1,2,2},{2,0,1},{2,1,2}};

static game default_game (){
    node nodes[EXAMPLE_NB_NODE];
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
    game g = new_game(EXAMPLE_NB_NODE, nodes,NB_BRIDGES_MAX,NB_DIR);
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      delete_node(nodes[i]);
    return g;
}

bool test_can_add_bridge_dir2() {
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

int main (int argc, char *argv[])
{
  printf("test4:\n");
  bool result= true;
  result = result && test_can_add_bridge_dir2();
  result ? printf(" test_can_add_bridge_dir2 ok \n") : printf(" test_can_add_bridge_dir2 not ok \n");
    if (result){
      fprintf(stdout,"test_can_add_bridge_dir2 success\n");
      return EXIT_SUCCESS;
    }

    else{
      fprintf(stderr,"test_can_add_bridge_dir2 failure\n");
      return EXIT_FAILURE;
    }
}
