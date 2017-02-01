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

bool test_get_neighbour() {
    game g = default_game();
    bool r = true;
    r = test_equality_int(5, get_neighbour_dir(g,0,EAST), " Pb neighbour 0 EAST") && r;
    r = test_equality_int(4, get_neighbour_dir(g,2,EAST), " Pb neighbour 2 EAST") && r;
    r = test_equality_int(0, get_neighbour_dir(g,1,SOUTH), " Pb neighbour 1 SOUTH") && r;
    r = test_equality_int(2, get_neighbour_dir(g,1,NORTH), " Pb neighbour 1 NORTH") && r;
    delete_game(g);
    return r;
}


int main (int argc, char *argv[])
{
   printf("test1:\n");
   bool result= true;
   result = test_get_neighbour() && result;
   result ? printf(" test_get_neighbour1 ok \n") : printf(" test_get_neighbour1 not ok \n");
   if (result){
      fprintf(stdout,"test_get_neighbour1 success\n");
      return EXIT_SUCCESS;
   }

   else{
      fprintf(stderr,"test_get_neighbour1 failure: get_neighbour1\n");
      return EXIT_FAILURE;
   }

}
