#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../include/game.h"
#include "test_toolbox.h"


#define EXAMPLE_NB_NODE 10
#define NB_BRIDGES_MAX 4
#define NB_DIR 8

/*
7   8   9   10

4   5       6



1   2       3

 */


int vals[EXAMPLE_NB_NODE][3] ={{0,0,1},{1,0,2},{3,0,3},{0,2,4},{1,2,5},{2,2,6},{0,3,7},{1,3,8},{2,3,9},{3,3,10}};


static game default_game (){
    node nodes[EXAMPLE_NB_NODE];
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
    game g = new_game(EXAMPLE_NB_NODE, nodes,NB_BRIDGES_MAX,NB_DIR);
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      delete_node(nodes[i]);
    return g;
}


bool test_can_add_bridge_dir(){
  game g = default_game();
  bool r = true;
  r = test_equality_bool(true,can_add_bridge_dir(g,4,EAST)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g,4,EAST);
  r = test_equality_bool(true,can_add_bridge_dir(g,4,EAST)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g,4,EAST);
  r = test_equality_bool(true,can_add_bridge_dir(g,4,EAST)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g,4,EAST);
  r = test_equality_bool(true,can_add_bridge_dir(g,4,EAST)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g,4,EAST);
  r = test_equality_bool(false,can_add_bridge_dir(g,4,EAST)," probleme de can_add_bride (pont de trop)\n") && r;
  return r;
}





int main (int argc, char *argv[])
{
    bool result= true;
    result = result && test_can_add_bridge_dir();
    if (result){
       fprintf(stdout,"test8 success\n");
       return EXIT_SUCCESS;
    }

    else{
      fprintf(stderr,"test8 failure:\n");
        return EXIT_FAILURE;
    }
}
