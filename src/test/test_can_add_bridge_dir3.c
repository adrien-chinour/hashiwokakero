#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../core/game.h"
#include "test_toolbox.h"
/*
  2   2   2

  2       2

  2   2   2



 */




#define EXAMPLE_NB_NODE 8
#define NB_BRIDGES_MAX 2
#define NB_DIR 8
int vals[EXAMPLE_NB_NODE][3] = {{0,0,2},{0,1,2},{0,2,2},{1,0,2},{1,2,2},{2,0,2},{2,1,2},{2,2,2}};

static game default_game (){
    node nodes[EXAMPLE_NB_NODE];
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
    game g = new_game(EXAMPLE_NB_NODE, nodes,NB_BRIDGES_MAX,NB_DIR);
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      delete_node(nodes[i]);
    return g;
}


bool test_can_add_bridge_dir3(){
   game g = default_game();
   bool r = true;
   r = test_equality_bool(true, can_add_bridge_dir(g, 0, NE), " can add NE") && r;
   add_bridge_dir(g, 0, NE);
   
   r = test_equality_bool(false, can_add_bridge_dir(g, 1, EAST), " can add EAST") && r;
   
   r = test_equality_bool(false, can_add_bridge_dir(g, 5, NW), " can add NW") && r;
   
   r = test_equality_bool(false, can_add_bridge_dir(g, 3, NORTH), " can add NORTH") && r;

   delete_game(g);
   return r;
}



int main (int argc, char *argv[])  
{
  printf("test6:\n");
  bool result= true;
  result = test_can_add_bridge_dir3() && result;
  result ? printf(" test_can_add_bridge_dir3 ok \n") : printf(" test_can_add_bridge_dir3 not ok \n");
  if (result){
    fprintf(stdout,"test_can_add_bridge_dir3 success\n");
    return EXIT_SUCCESS;
  }
  
    else{
      fprintf(stderr,"test_can_add_bridge_dir3 failure\n");
      return EXIT_FAILURE;
    }
}
