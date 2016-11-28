#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/node.h"
#include "../include/game.h"

#define NB_NODES 7

game generate_game(int nb_game, int nb_max_bridges, int nb_dir){
	
	int game_un[NB_NODES][3] = {{0,0,3},{0,2,5},{0,4,2},{2,2,1},{2,4,2},{4,0,2},{4,4,3}};

	node nodes[NB_NODES];
    for (int i = 0 ; i < NB_NODES; i++){
    	switch(nb_game){
    		case 1: 
                nodes[i] = new_node(game_un[i][0],game_un[i][1],game_un[i][2]);
                break;
    	}
    }

    game g = new_game(NB_NODES, nodes, nb_max_bridges, nb_dir);
    
    for (int i = 0 ; i < NB_NODES; i++)
      delete_node(nodes[i]);
    
    return g;
}