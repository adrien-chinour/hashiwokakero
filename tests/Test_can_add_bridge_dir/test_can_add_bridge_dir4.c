#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../include/game.h"
#include "test_toolbox.h"


/* Attention ce jeu n'est pas valide, il ne peut être utilisé que dans ce test !

  2   3

  3      1

      4
*/

#define EXAMPLE_NB_NODE 5
#define NB_BRIDGES_MAX 2
#define NB_DIR 4

int vals[EXAMPLE_NB_NODE][3] = {{0,1,3},{0,2,2},{1,0,4},{1,2,3},{2,1,1}};

static game default_game (){
    node nodes[EXAMPLE_NB_NODE];
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
    game g = new_game(EXAMPLE_NB_NODE, nodes,NB_BRIDGES_MAX,NB_DIR);
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      delete_node(nodes[i]);
    return g;
}



bool test_can_add_bridge_dir4() {
    game g = default_game();
    bool r = true;
    //int num = game_get_node_number (g, 0, 0);
    r = test_equality_bool(true, can_add_bridge_dir(g, 0, EAST), "pb de pont simple à l'est") && r;
    add_bridge_dir(g, 0, EAST);
    r = test_equality_bool(false, can_add_bridge_dir(g, 2, NORTH), "pb de pont se traversant") && r;
    r = test_equality_bool(true, can_add_bridge_dir(g, 1, SOUTH), "pb de pont sur une node en possédant déjà un") && r;
    delete_game(g);
    return r;
}

int main (int argc, char *argv[])
{
    printf("test7:\n");
    bool result= true;
    result = test_can_add_bridge_dir4() && result;
    result ? printf(" test_can_add_bridge_dir4 ok \n") : printf(" test_can_add_bridge_dir4 not ok \n");
    if (result){
       fprintf(stdout,"test_can_add_bridge_dir4 success\n");
        return EXIT_SUCCESS;
    }

    else{
       fprintf(stderr,"test_can_add_bridge_dir4 failure\n");
       return EXIT_FAILURE;
    }

}
