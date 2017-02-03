#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../core/game.h"
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


bool test_get_neighbour2() {
    game g = default_game();
    bool r = true;
    r = r && test_equality_int(3, get_neighbour_dir(g,0,EAST), "pb neighbour 0 EAST");
    r = r && test_equality_int(5, get_neighbour_dir(g,2,EAST), "pb neighbour 2 EAST");
    r = r && test_equality_int(0, get_neighbour_dir(g,1,SOUTH), "pb neighbour 1 SOUTH");
    r = r && test_equality_int(2, get_neighbour_dir(g,1,NORTH), "pb neighbour 1 NORTH");
    delete_game(g);
    return r;
}



int main ()
{
   printf("%d\n", test_get_neighbour2());
   return EXIT_SUCCESS;;
}
