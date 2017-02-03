#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../core/game.h"
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


bool test_can_add_bridge_dir5(){
  game g = default_game();
  bool r = true;
  r = test_equality_bool(true,can_add_bridge_dir(g,3,EAST)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g,3,EAST);
  r = test_equality_bool(true,can_add_bridge_dir(g,3,EAST)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g,3,EAST);
  r = test_equality_bool(true,can_add_bridge_dir(g,3,EAST)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g,3,EAST);
  r = test_equality_bool(true,can_add_bridge_dir(g,3,EAST)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g,3,EAST);
  r = test_equality_bool(false,can_add_bridge_dir(g,3,EAST)," probleme de can_add_bride\n") && r;

  r = test_equality_bool(true,can_add_bridge_dir(g,6,SE)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g, 6, SE);

  r = test_equality_bool(true,can_add_bridge_dir(g,7,SOUTH)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g, 7, SOUTH);

  r = test_equality_bool(true,can_add_bridge_dir(g,8,SW)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g,8,SW);

  r = test_equality_bool(true,can_add_bridge_dir(g,5,WEST)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g, 5, WEST);

  r = test_equality_bool(true,can_add_bridge_dir(g,2,NW)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g, 2, NW);

  r = test_equality_bool(true,can_add_bridge_dir(g,1,NORTH)," probleme de can_add_bride\n") && r;
  add_bridge_dir(g, 1, NORTH);

  r = test_equality_bool(false,can_add_bridge_dir(g,0,NE)," probleme de can_add_bride\n") && r;

  delete_game(g);
  return r;
}





int main ()
{
   printf("%d", test_can_add_bridge_dir5());
   return EXIT_SUCCESS;;
}
