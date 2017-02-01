#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../include/game.h"
#include "test_toolbox.h"


#define EXAMPLE_NB_NODE 8
#define NB_BRIDGES_MAX 2
#define NB_DIR 8

/*
 2  .  .  .  2
 .  \\ .  /  |
 2  -  5  .  1
 |  .  .  \  .
 3  =  2  .  1
*/

int vals[EXAMPLE_NB_NODE][3] = {{0,0,3},{0,2,2},{0,4,2},{2,0,2},{2,2,5},{4,0,1},{4,2,1},{4,4,2}};

static game default_game (){
    node nodes[EXAMPLE_NB_NODE];
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
    game g = new_game(EXAMPLE_NB_NODE, nodes,NB_BRIDGES_MAX,NB_DIR);
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      delete_node(nodes[i]);
    return g;
}

bool test_copy_game_bridges(){
   game g=default_game();
   bool r=true;
   add_bridge_dir(g, 0, EAST);
   add_bridge_dir(g, 0, EAST);
   add_bridge_dir(g, 0, NORTH);
   add_bridge_dir(g, 1, EAST);
   add_bridge_dir(g, 2, SE);
   add_bridge_dir(g, 2, SE);
   game g_copy=copy_game(g);
   r = r && test_equality_bool(false, game_over(g), "  Pb game_over");
   r = r && test_equality_bool(false, game_over(g_copy), "  Pb game_over on copy");
   delete_game(g_copy);
   add_bridge_dir(g, 4, NE);
   add_bridge_dir(g, 4, SE);
   add_bridge_dir(g, 7, SOUTH);
   g_copy=copy_game(g);
   r = r && test_equality_bool(true, game_over(g), "  Pb game_over");
   r = r && test_equality_bool(true, game_over(g_copy), "  Pb game_over on copy");
   delete_game(g);
   delete_game(g_copy);
   return r;
}







int main (int argc, char *argv[]){
   printf("test9:\n");
   bool result = true;
   result = result && test_copy_game_bridges();
   result ? printf(" test_copy_game_bridges ok \n") : printf(" test_copy_game_bridges not ok \n");

    if (result){
       fprintf(stdout,"test_copy_game_bridges success\n");
        return EXIT_SUCCESS;
    }
    else{
       fprintf(stderr,"test_copy_game_bridges failure\n");
       return EXIT_FAILURE;
    }
}
