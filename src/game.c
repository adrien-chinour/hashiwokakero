#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/node.h"

typedef enum dir_e {NORTH, WEST, SOUTH, EAST} dir;

#define NB_DIRS 4

typedef struct game_s {
	int nb_nodes;
	node *nodes;
	int **bridges;
} *game;

typedef const struct game_s* cgame;

game new_game (int nb_nodes, node *nodes){

	game g = (game) malloc(sizeof(struct game_s));
	
	g->nb_nodes = nb_nodes;
	
	node *new_nodes = (node*) malloc(nb_nodes*sizeof(node)); 
	for(int i = 0; i < nb_nodes; i++){
    new_nodes[i] = new_node(get_x(nodes[i]), get_y(nodes[i]), get_required_degree(nodes[i]));
  }
	g->nodes = new_nodes;

	// http://www.commentcamarche.net/forum/affich-4931713-tableau-dynamique-a-2-dimensions
	int **bridges;
	bridges = (int**) malloc(nb_nodes*sizeof(int*)); 
	for(int i = 0; i < nb_nodes; i++){
		bridges[i] = (int*) malloc(NB_DIRS*sizeof(int)); 
	}

	for(int i = 0; i < nb_nodes; i++){
		for(int j = 0; j < NB_DIRS; j++){
			bridges[i][j] = 0;
		}
	}

	g->bridges = bridges;

	return g;
}

void delete_game (game g){
    for(int i = 0; i < g->nb_nodes; i++){
      delete_node(g->nodes[i]);
      free(g->bridges[i]);
    }
  free(g->nodes);
  free(g->bridges);
  free(g);
  g = NULL;
}

game copy_game (cgame g_src){

   int nb_nodes = g_src->nb_nodes;
	game g = (game) malloc(sizeof(struct game_s));
	
	g->nb_nodes = nb_nodes;
	
	node *new_nodes = (node*) malloc(nb_nodes*sizeof(node));
  for(int i = 0; i < nb_nodes; i++){
    new_nodes[i] = new_node(get_x(g_src->nodes[i]), get_y(g_src->nodes[i]), get_required_degree(g_src->nodes[i]));
  }
	g->nodes = new_nodes;

	// http://www.commentcamarche.net/forum/affich-4931713-tableau-dynamique-a-2-dimensions
   int **bridges;
   bridges = malloc(nb_nodes*sizeof(int*)); 
   for(int i = 0; i < nb_nodes; i++){
      bridges[i] = malloc(NB_DIRS*sizeof(int)); 
   }
	for(int i = 0; i < nb_nodes; i++){
		for(int j = 0; j < NB_DIRS; j++){
			bridges[i][j] = g_src->bridges[i][j];
		}
	}

	g->bridges = bridges;

	return g;
}

int game_nb_nodes (cgame g){
   return g->nb_nodes;
}

node game_node (cgame g, int node_num){
  if(node_num >= 0 && node_num < game_nb_nodes(g))
    return g->nodes[node_num];
  else{
    printf("Erreur : il n'y a pas de node %d \n", node_num);
    exit(EXIT_FAILURE);
  }
}

//la fonction game_over est à la fin, je la signale ici
bool game_over (cgame g);

//la fonction can_add_bridge_dir est à la fin, je le signale ici
bool can_add_bridge_dir (cgame g, int node_num, dir d);

//idem
int get_neighbour_dir (cgame g, int node_num, dir d);

void add_bridge_dir (game g, int node_num, dir d){
  if(can_add_bridge_dir (g, node_num, d)){
    g->bridges[node_num][d]++;
    switch(d){
    case SOUTH:
      g->bridges[get_neighbour_dir(g, node_num, d)][NORTH]++;
      break;
    case NORTH:
      g->bridges[get_neighbour_dir(g, node_num, d)][SOUTH]++;
      break;
    case EAST:
      g->bridges[get_neighbour_dir(g, node_num, d)][WEST]++;
      break;
    case WEST:
      g->bridges[get_neighbour_dir(g, node_num, d)][EAST]++;
      break;
    }
  }
  else {
      printf("Erreur: can_add_bridge_dir (g, node_num, d) n'est pas valide.\n");
      exit(EXIT_FAILURE);
  }
}

void del_bridge_dir (game g, int node_num, dir d){
   if(g->bridges[node_num][d] != 0){
    switch(d){
      case SOUTH:
        g->bridges[get_neighbour_dir(g, node_num, d)][NORTH]--;
        break;
      case NORTH:
        g->bridges[get_neighbour_dir(g, node_num, d)][SOUTH]--;
        break;
      case EAST:
        g->bridges[get_neighbour_dir(g, node_num, d)][WEST]--;
        break;
      case WEST:
        g->bridges[get_neighbour_dir(g, node_num, d)][EAST]--;
        break;
    }
    g->bridges[node_num][d]--;
   }
}

int get_degree_dir (game g, int node_num, dir d){
   return g->bridges[node_num][d];
}

int get_degree(cgame g, int node_num){
   int k = 0;
   for(int i = 0; i < NB_DIRS; i++){
     k = k + g->bridges[node_num][i];
   }
   return k;
}

//cette fonction est écrite plus bas, elle est référencée ici
int game_get_node_number (cgame g, int x, int y);

int get_neighbour_dir (cgame g, int node_num, dir d){
  node n = game_node(g, node_num);
  switch(d){
    case NORTH: 
    {
      int max_y = 0;
      for(int i = 0; i < g->nb_nodes; i++){
        if(get_y(g->nodes[i]) > max_y)
          max_y = get_y(g->nodes[i]);
      }

      int y = get_y(n)+1;
      while (game_get_node_number(g,get_x(n),y) == -1 && y <= max_y){
        y++;  
      }
      if(game_get_node_number(g,get_x(n),y) != -1)
        return game_get_node_number(g,get_x(n),y);
      break; 
    }

    case EAST: 
    {
      int max_x = 0;
      for(int i = 0; i < g->nb_nodes; i++){
        if(get_x(g->nodes[i]) > max_x)
          max_x = get_x(g->nodes[i]);
      }

      int x = get_x(n)+1;
      while (game_get_node_number(g,x,get_y(n)) == -1 && x <= max_x){
        x++;  
      }
      if(game_get_node_number(g,x,get_y(n)) != -1)
        return game_get_node_number(g,x,get_y(n));
      break; 
    }

    case SOUTH:
    {
      int y = get_y(n)-1;
      while(game_get_node_number(g,get_x(n),y) == -1 && y >= 0){
        y--;
      }
      if(game_get_node_number(g,get_x(n),y) != -1)
        return game_get_node_number(g,get_x(n),y);
      break;
    }

    case WEST:
    {
      int x = get_x(n)-1;
      while(game_get_node_number(g,x,get_y(n)) == -1 && x >= 0){
        x--;
      }
      if(game_get_node_number(g,x,get_y(n)) != -1)
        return game_get_node_number(g,x,get_y(n));
      break;
    }
  
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


//this function prepare to verify if the game is connexe
void recursive(cgame g, int number, int mark[])
{
  if(mark[number] != 1) 
  {
    mark[number] = 1;
    for(int i = 0; i < NB_DIRS; i++)
    {
      int neighbour = get_neighbour_dir(g, number, i);
      if(neighbour != -1)
        recursive(g, neighbour, mark);
    }
  }
}

bool game_over (cgame g){

//initialize mark[] (which will mark all the nodes linked) to 0
  int mark[g->nb_nodes];
  for(int i = 0; i < g->nb_nodes; i++)
    mark[i] = 0;
  
  //mark all the nodes linked by the same group of bridges
  recursive(g, 0, mark);
  for(int i = 0; i < g->nb_nodes; i++){
     if(mark[i] != 1)
        return true;
  }
  return false;
}

bool can_add_bridge_dir (cgame g, int node_num, dir d){

  if( get_neighbour_dir(g, node_num, d) == -1) {
    return false;
  }

  if(g->bridges[node_num][d] < 2){
      switch (d){

               //    C
               //    |
               // A--+--B
               //    |
               //    D

         case NORTH:
            for(int i = 0; i < g->nb_nodes; i++){
               node n = game_node(g, i);

               if(g->bridges[i][EAST] > 0 // bridge A -> B
                  && get_x(n) > get_x(game_node(g, node_num)) // xA > xD
                  && get_x(n) < get_x(game_node(g, get_neighbour_dir(g, node_num, NORTH))) // xA < xC
                  && get_y(n) < get_y(game_node(g, node_num)) // yA < yD
                  && get_y(game_node(g, get_neighbour_dir(g, i, EAST))) > get_y(game_node(g, node_num)) ) // yB > yD
                  return false;

               if(g->bridges[i][WEST] > 0 // bridge B -> A
                  && get_x(n) > get_x(game_node(g, node_num)) // xB > xD
                  && get_x(n) < get_x(game_node(g, get_neighbour_dir(g, node_num, NORTH))) //xB < xC
                  && get_y(n) > get_y(game_node(g, node_num)) // yB > yD
                  && get_y(game_node(g, get_neighbour_dir(g, i, WEST))) < get_y(game_node(g, node_num)) ) //yA < yD
                  return false;
            }
            return true;
            break;

         case WEST:
            for(int i = 0; i < g->nb_nodes; i++){
               node n = game_node(g, i);

               if(g->bridges[i][SOUTH] > 0 // bridge C -> D
                  && get_x(n) > get_x(game_node(g, node_num)) // xC > xA
                  && get_x(game_node(g ,get_neighbour_dir(g, i, SOUTH))) < get_x(game_node(g, node_num)) // xD < xA
                  && get_y(n) > get_y(game_node(g, node_num)) // yC > yA
                  && get_y(n) < get_y(game_node(g, get_neighbour_dir(g, node_num, WEST))) ) // yC < yB
                  return false;

               if(g->bridges[NORTH][i] > 0 // bridge D -> C
                  && get_x(n) < get_x(game_node(g, node_num)) // xD < xA
                  && get_x(game_node(g, get_neighbour_dir(g, i, NORTH))) < get_x(game_node(g, node_num)) //xC > xA
                  && get_y(n) > get_y(game_node(g, node_num)) // yD > yA
                  && get_y(n) < get_y(game_node(g, get_neighbour_dir(g, node_num, WEST))) ) //yD < yB
                  return false;
            }
            return true;
            break;

         case SOUTH:
            for(int i = 0; i < g->nb_nodes; i++){
               node n = game_node(g, i);

               if(g->bridges[i][EAST] > 0 // bridge A -> B
                  && get_x(n) < get_x(game_node(g, node_num)) // xA < xC
                  && get_x(n) > get_x(game_node(g, get_neighbour_dir(g, node_num, SOUTH))) // xA > xD
                  && get_y(n) < get_y(game_node(g, node_num)) // yA < yC
                  && get_y(game_node(g, get_neighbour_dir(g, i, EAST))) > get_y(game_node(g, node_num)) ) // yB > yC
                  return false;

               if(g->bridges[i][WEST] > 0 // bridge B -> A
                  && get_x(n) < get_x(game_node(g, node_num)) // xB < xC
                  && get_x(n) > get_x(game_node(g, get_neighbour_dir(g, node_num, SOUTH))) //xB > xD
                  && get_y(n) > get_y(game_node(g, node_num)) // yB > yC
                  && get_y(game_node(g, get_neighbour_dir(g, i, WEST))) < get_y(game_node(g, node_num)) ) //yA < yC
                  return false;
            }
            return true;
            break;

         case EAST:
            for(int i = 0; i < g->nb_nodes; i++){
               node n = game_node(g, i);

               if(g->bridges[i][SOUTH] > 0 // bridge C -> D
                  && get_x(n) > get_x(game_node(g, node_num)) // xC > xB
                  && get_x(game_node(g ,get_neighbour_dir(g, i, SOUTH))) < get_x(game_node(g, node_num)) // xD < xC
                  && get_y(n) < get_y(game_node(g, node_num)) // yC < yB
                  && get_y(n) > get_y(game_node(g, get_neighbour_dir(g, node_num, WEST))) ) // yC > yA
                  return false;

               if(g->bridges[i][NORTH] > 0 // bridge D -> C
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
  return false;
}
