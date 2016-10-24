#include "../include/node.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define NB_DIRS 4

typedef enum dir_e {NORTH, WEST, SOUTH, EAST} dir;

typedef struct game_s{
   node *nodes;
   int nb_nodes;
   int bridges[][NB_DIRS];
} *game;

typedef const struct game_s* cgame;

game new_game(int nb_nodes,node *nodes){
   game ngame = (game) malloc(sizeof(game));
   ngame->nodes = nodes;
   ngame->nb_nodes = nb_nodes;
   return ngame;
}

void delete_game (game g){
   free(g->nodes);
   free(g);
}

game copy_game (cgame g_src){
   node *nodes = g_src->nodes;
   int nb_nodes = g_src->nb_nodes;
   int bridges[nb_nodes][NB_DIRS];
   game ngame = new_game(nb_nodes, nodes);
   for(int i = 0; i < nb_nodes; i++){
      for(int j = 0; j < NB_DIRS; j++){
         ngame->bridges[i][j] = g_src->bridges[i][j];
      }
   }
   return ngame;
}

int game_nb_nodes (cgame g){
   return g->nb_nodes;
}

node game_node (cgame g, int node_num){
   if(node_num > 0 && node_num < game_nb_nodes(g)){
      return *((g->nodes)+node_num);
   }
   //ajouter le cas ou node_num > a nb_nodes ou < a 0
   exit;
}

void add_bridge_dir (game g, int node_num, dir d){
   g->bridges[node_num][d]++;
}

void del_bridge_dir (game g, int node_num, dir d){
   if(g->bridges[node_num][d] != 0)
      g->bridges[node_num][d]--;      
}

int get_degree_dir (game g, int node_num, dir d){
   return g->bridges[node_num][d];
}

int get_degree(cgame g, int node_num){
   int k = 0;
   for(int i = 0; i < NB_DIRS; i++){
      k += g->bridges[node_num][i];
   }
   return k;
}

bool game_over (cgame g){
   for(int i = 0; i < g->nb_nodes; i++){
      node n = game_node(g, i);
      if( get_degree(g, i) != get_required_degree(n))
         return false;
   }
   return true;
}

int get_neighbour_dir (cgame g, int node_num, dir d){
   node n = game_node(g, node_num);
   switch (d){
      case 0: //NORTH
         for(int i = 0; i < g->nb_nodes; i++){
            node nc = game_node(g, i);
            if(get_x(n) < get_x(nc) && get_y(n) == get_y(nc)){
               return i;
            }
         }
         break;

      case 1: //WEST
         for(int i = 0; i < g->nb_nodes; i++){
            node nc = game_node(g, i);
            if(get_x(n) == get_x(nc) && get_y(n) > get_y(nc)){
               return i;
            }
         }
         break;

      case 2: //SOUTH
         for(int i = 0; i < g->nb_nodes; i++){
            node nc = game_node(g, i);
            if(get_x(n) > get_x(nc) && get_y(n) == get_y(nc)){
               return i;
            }
         }
         break;

      case 3: //EAST
         for(int i = 0; i < g->nb_nodes; i++){
            node nc = game_node(g, i);
            if(get_x(n) == get_x(nc) && get_y(n) < get_y(nc)){
               return i;
            }
         }
         break;
   }
   return -1;
}

int game_get_node_number (cgame g, int x, int y){
   for(int i = 0; i < g->nb_nodes; i++){
      node n = game_node(g, i);
      if(get_x(n) == x && get_y(n) == y)
         return i;
   }
   return -1;
}

bool can_add_bridge_dir (cgame g, int node_num, dir d){
   if( g->bridges[node_num][d] < 2){
      switch (d){

               //    C
               //    |
               // A--+--B
               //    |
               //    D

         case 0:
            for(int i = 0; i < g->nb_nodes; i++){
               node n = game_node(g, i);

               if(g->bridges[i][3] > 0 // bridge A -> B
                  && get_x(n) > get_x(game_node(g, node_num)) // xA > xD
                  && get_x(n) < get_x(game_node(g, get_neighbour_dir(g, node_num, NORTH))) // xA < xC
                  && get_y(n) < get_y(game_node(g, node_num)) // yA < yD
                  && get_y(game_node(g, get_neighbour_dir(g, i, EAST))) > get_y(game_node(g, node_num)) ) // yB > yD
                  return false;

               if(g->bridges[i][1] > 0 // bridge B -> A
                  && get_x(n) > get_x(game_node(g, node_num)) // xB > xD
                  && get_x(n) < get_x(game_node(g, get_neighbour_dir(g, node_num, NORTH))) //xB < xC
                  && get_y(n) > get_y(game_node(g, node_num)) // yB > yD
                  && get_y(game_node(g, get_neighbour_dir(g, i, WEST))) < get_y(game_node(g, node_num)) ) //yA < yD
                  return false;
            }
            return true;
            break;

         case 1:
            for(int i = 0; i < g->nb_nodes; i++){
               node n = game_node(g, i);

               if(g->bridges[i][2] > 0 // bridge C -> D
                  && get_x(n) > get_x(game_node(g, node_num)) // xC > xA
                  && get_x(game_node(g ,get_neighbour_dir(g, i, SOUTH))) < get_x(game_node(g, node_num)) // xD < xA
                  && get_y(n) > get_y(game_node(g, node_num)) // yC > yA
                  && get_y(n) < get_y(game_node(g, get_neighbour_dir(g, node_num, WEST))) ) // yC < yB
                  return false;

               if(g->bridges[i][0] > 0 // bridge D -> C
                  && get_x(n) < get_x(game_node(g, node_num)) // xD < xA
                  && get_x(game_node(g, get_neighbour_dir(g, i, NORTH))) < get_x(game_node(g, node_num)) //xC > xA
                  && get_y(n) > get_y(game_node(g, node_num)) // yD > yA
                  && get_y(n) < get_y(game_node(g, get_neighbour_dir(g, node_num, WEST))) ) //yD < yB
                  return false;
            }
            return true;
            break;

         case 2:
            for(int i = 0; i < g->nb_nodes; i++){
               node n = game_node(g, i);

               if(g->bridges[i][3] > 0 // bridge A -> B
                  && get_x(n) < get_x(game_node(g, node_num)) // xA < xC
                  && get_x(n) > get_x(game_node(g, get_neighbour_dir(g, node_num, SOUTH))) // xA > xD
                  && get_y(n) < get_y(game_node(g, node_num)) // yA < yC
                  && get_y(game_node(g, get_neighbour_dir(g, i, EAST))) > get_y(game_node(g, node_num)) ) // yB > yC
                  return false;

               if(g->bridges[i][1] > 0 // bridge B -> A
                  && get_x(n) < get_x(game_node(g, node_num)) // xB < xC
                  && get_x(n) > get_x(game_node(g, get_neighbour_dir(g, node_num, SOUTH))) //xB > xD
                  && get_y(n) > get_y(game_node(g, node_num)) // yB > yC
                  && get_y(game_node(g, get_neighbour_dir(g, i, WEST))) < get_y(game_node(g, node_num)) ) //yA < yC
                  return false;
            }
            return true;
            break;

         case 3:
            for(int i = 0; i < g->nb_nodes; i++){
               node n = game_node(g, i);

               if(g->bridges[i][2] > 0 // bridge C -> D
                  && get_x(n) > get_x(game_node(g, node_num)) // xC > xB
                  && get_x(game_node(g ,get_neighbour_dir(g, i, SOUTH))) < get_x(game_node(g, node_num)) // xD < xC
                  && get_y(n) < get_y(game_node(g, node_num)) // yC < yB
                  && get_y(n) > get_y(game_node(g, get_neighbour_dir(g, node_num, WEST))) ) // yC > yA
                  return false;

               if(g->bridges[i][0] > 0 // bridge D -> C
                  && get_x(n) < get_x(game_node(g, node_num)) // xD < xB
                  && get_x(game_node(g, get_neighbour_dir(g, i, NORTH))) < get_x(game_node(g, node_num)) //xC > xB
                  && get_y(n) < get_y(game_node(g, node_num)) // yD < yA
                  && get_y(n) > get_y(game_node(g, get_neighbour_dir(g, node_num, WEST))) ) //yD > yA
                  return false;
            }
            return true;
            break;

      }
   }
}