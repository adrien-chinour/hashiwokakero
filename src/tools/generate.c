#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../solver/solver.h"

/*
  Générateur de map 3 * 3
  Argument de la fonction : max_bridges ; nb_dir
  Fonctionnement : génération aléatoire de 9 noeuds avec un degré entre 1 et max_bridges * nb_dir
*/

node * generate_nodes(int nb_max_bridges, int nb_dir){
   node * nodes = (node*) malloc(9*sizeof(node));
   int k = 0;
   for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
         int degree = rand()%(nb_max_bridges*nb_dir)+1;
         nodes[k] = new_node(i,j,degree);
         k++;
      }
   }
   return nodes;
}

bool test_game(game g, int nb_max_bridges, int nb_dir){
   simple_bridges(g);
   bool * go=malloc(sizeof(bool));
   *go=false;

   if(solver_r(g,0,-1,go)){
      free(go);
      return true;
   }
   else {
      free(go);
      return false;
   }
}

game random_game(int nb_max_bridges, int nb_dir){
   srand(time(NULL));
   game g = NULL;
   do {
      if(g != NULL){ delete_game(g); g=NULL;}
      node * nodes = generate_nodes(nb_max_bridges, nb_dir);
      g = new_game(9, nodes, nb_max_bridges, nb_dir);
      for(int i = 0; i < 9; i++)
         delete_node(nodes[i]);
      free(nodes);
   } while(!test_game(g, nb_max_bridges, nb_dir));
   for(int i = 0; i < game_nb_nodes(g); i++){
      for(int j = 0; j < game_nb_dir(g); j++){
         while(get_degree_dir(g, i, j) != 0){
            del_bridge_dir(g, i, j);
         }
      }
   }
   return g;
}
