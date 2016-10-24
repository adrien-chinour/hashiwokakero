#include "../include/node.h"
#include <stdlib.h>

typedef enum dir_e {NORTH, WEST, SOUTH, EAST} dir;

typedef struct game_s{
   int nbnode;
   int* matrix;
}game;
typedef const struct game_s* cgame;

game new_game(int nb_nodes,node *nodes){
   game* Game ;
   (*Game).nbnode=nb_nodes;
   //(*Game).matrix=malloc(sizeof(int));
   return *Game;
}

void delete_game(game g){
}

game copy_game(cgame g_src){
   return *g_src;
}

int game_nb_nodes(cgame g){
   return -1;
}

node game_node(cgame g, int node_num){
   return new_node(0,0, 0);
}

bool game_over(cgame g){
   return false;
}

bool can_add_bridge_dir (cgame g, int node_num, dir d){
   return false;
}

void add_bridge_dir (game g, int node_num, dir d){
}

void del_bridge_dir (game g, int node_num, dir d){
}

int get_degree_dir (cgame g, int node_num, dir d){
      return -1;
}

int get_degree (cgame g, int node_num){
   return -1;
}

int get_neighbour_dir (cgame g, int node_num, dir d){
   return -1;
}

int game_get_node_number (cgame g, int x, int y){
   return -1;
}

int main(void){
   return EXIT_SUCCESS;
}
