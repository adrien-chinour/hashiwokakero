#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/node.h"
#include "../include/game.h"

// Les tabeaux utilisés pour généré les games sont déclarés ici.
int game1[7][3] = {{0,0,3},{0,2,5},{0,4,2},{2,2,1},{2,4,2},{4,0,2},{4,4,3}};
int game2[8][3] = {{0,0,3},{0,2,2},{0,4,2},{2,0,2},{2,2,5},{4,0,1},{4,2,1},{4,4,2}};


// fin déclaration des tableaux.

game generate_game(int nb_game, int nb_max_bridges, int nb_dir, int nb_nodes){
	
    node nodes[nb_nodes];
    for (int i = 0 ; i < nb_nodes; i++){
    	switch(nb_game){
    	  case 1: nodes[i] = new_node(game1[i][0],game1[i][1],game1[i][2]); break;
        case 2: nodes[i] = new_node(game2[i][0],game2[i][1],game2[i][2]); break;
        /*case 3: nodes[i] = new_node(game3[i][0],game3[i][1],game3[i][2]); break;
        case 4: nodes[i] = new_node(game4[i][0],game4[i][1],game4[i][2]); break;
        case 5: nodes[i] = new_node(game5[i][0],game5[i][1],game5[i][2]); break;
        case 6: nodes[i] = new_node(game6[i][0],game6[i][1],game6[i][3]); break;*/ 
        }
    }

    game g = new_game(nb_nodes, nodes, nb_max_bridges, nb_dir);

    for (int i = 0 ; i < nb_nodes; i++) delete_node(nodes[i]);
    
    return g;
}