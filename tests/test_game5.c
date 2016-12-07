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



bool test_pont_equivalent(){
   game g = default_game();
   bool r = true;
   add_bridge_dir(g, 1, NORTH);
   r = r && test_equality_int(get_degree(g, 1), get_degree(g, 2), "le degré n'est pas équivalent");
   delete_game(g);
   return r;
}


int main (int argc, char *argv[])
{
    bool result= true;
    result = result && test_pont_equivalent();
    if (result){
       fprintf(stdout,"test5 success\n");
       return EXIT_SUCCESS;
    }
    else{
       fprintf(stderr,"test5 failure\n");
       return EXIT_FAILURE;
    }

}
