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
int vals[EXAMPLE_NB_NODE][3] = {{0,0,3},{0,1,5},{0,2,2},{1,1,1},{1,2,2},{2,0,2},{2,2,3}};

static game default_game (){
    node nodes[EXAMPLE_NB_NODE];
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
    game g = new_game(EXAMPLE_NB_NODE, nodes);
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      delete_node(nodes[i]);
    return g;
}

bool test_copy_game(){
   cgame original=default_game();
   bool r=true;
   game copy= copy_game(original);
   r = r && test_equality_int((game_get_node_number(original, 0, 0)),game_get_node_number(copy, 0, 0),"les nodes ne correspondent pas");
   dir d;
   int c;
   for(int i=0;i<EXAMPLE_NB_NODE;i++){
      c=0;
      while(c<5){
         d=c;
         r = r && test_equality_int(get_neighbour_dir(original,i,d ),get_neighbour_dir(copy,i,d),"");
         c = c+1;
   }
      }
   return r;
}



int main (int argc, char *argv[]){
   bool result = true;
   result = result && test_copy_game();
   if(result)
      return EXIT_SUCCESS;
   else
      return EXIT_FAILURE;

}
