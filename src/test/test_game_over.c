#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../core/game.h"
#include "test_toolbox.h"

/* Defalult configuration
   2     2

   2     2
*/

#define EXAMPLE_NB_NODE 4
#define NB_BRIDGES_MAX 2
#define NB_DIR 4
int vals[EXAMPLE_NB_NODE][3] = {{0,0,2},{0,1,2},{1,0,2},{1,1,2}};

static game default_game (){
    node nodes[EXAMPLE_NB_NODE];
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
    game g = new_game(EXAMPLE_NB_NODE, nodes,NB_BRIDGES_MAX,NB_DIR);
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      delete_node(nodes[i]);
    return g;
}



bool test_game_over(){
   game g =default_game();
   bool r = true;
   add_bridge_dir(g,0,EAST);
   r = r && test_equality_bool(false, game_over(g),"Pb game_over degree");
   add_bridge_dir(g,0,EAST);
   add_bridge_dir(g,1,EAST);
   add_bridge_dir(g,1,EAST);
   r = r && test_equality_bool(false, game_over(g),"Pb game_over connexité");
   del_bridge_dir(g, 0, EAST);
   del_bridge_dir(g, 1, EAST);
   add_bridge_dir(g, 0, NORTH);
   add_bridge_dir(g, 2, NORTH);
   r = r && test_equality_bool(true, game_over(g),"Pb game_over autre");
   delete_game(g);
   return r;
}


int main ()
{
   printf("%d\n", test_game_over());
   return EXIT_SUCCESS;;
}
