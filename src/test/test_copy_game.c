#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../core/game.h"
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




bool test_copy_game(){
   game g=default_game();
   bool r=true;
   add_bridge_dir(g,1,NORTH);
   add_bridge_dir(g,1,NORTH);
   add_bridge_dir(g,3,NORTH);
   game g_copy = copy_game(g);
   r = r && test_equality_int((game_get_node_number(g, 0, 0)),game_get_node_number(g_copy, 0, 0),"nodes doesn't match copy");
   dir d;
   int c;
   for(int i=0;i<EXAMPLE_NB_NODE;i++){
      c=0;
      r = r && test_equality_int(get_degree(g,i),get_degree(g_copy,i),"pb degree copy");
      while(c<5){
         d=c;
         r = r && test_equality_int(get_neighbour_dir(g,i,d ),get_neighbour_dir(g_copy,i,d),"pb neighbour copy");
         c = c+1;
      }
    }
    delete_game(g);
    delete_game(g_copy);
   return r;
}



int main ()
{
   printf("%d", test_copy_game());
   return EXIT_SUCCESS;;
}
