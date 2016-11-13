#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../include/game.h"
#include "test_toolbox.h"

/* Defalult configuration
   2     2

   2     2
*/

#define EXAMPLE_NB_NODE 4
int vals[EXAMPLE_NB_NODE][3] = {{0,0,2},{0,1,2},{1,0,2},{1,1,2}};

static game default_game (){
    node nodes[EXAMPLE_NB_NODE];
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
    game g = new_game(EXAMPLE_NB_NODE, nodes);
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      delete_node(nodes[i]);
    return g;
}

bool test_game_over(){
   game g =default_game();
   bool r = true;
   add_bridge_dir(g,0,EAST);
   r = r && test_equality_bool(false, game_over(g),"Pb game_over");
   add_bridge_dir(g,0,EAST);
   add_bridge_dir(g,1,EAST);
   add_bridge_dir(g,1,EAST);
   r = r && test_equality_bool(false, game_over(g),"Pb game_over");
   del_bridge_dir(g, 0, EAST);
   del_bridge_dir(g, 1, EAST);
   add_bridge_dir(g, 0, NORTH);
   add_bridge_dir(g, 3, SOUTH);
   r = r && test_equality_bool(true, game_over(g),"Pb game_over");
   delete_game(g);
   return r;
}


int main (int argc, char *argv[])
{
    bool result= true;
    result = result && test_game_over();
    if (result)
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}