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
     free(g->bridges[i]);
     delete_node(g->nodes[i]);
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
    g->bridges[get_neighbour_dir(g, node_num, d)][d]++;
  }
  else {
      printf("Erreur: can_add_bridge_dir (g, node_num, d) n'est pas valide.\n");
      exit(EXIT_FAILURE);
  }
}

void del_bridge_dir (game g, int node_num, dir d){
   if(g->bridges[node_num][d] != 0){
      g->bridges[get_neighbour_dir(g,node_num,d)][d]--;
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


//cette fonction récursive prépare la vérification de la connexité
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

//initialise mark[] (qui marquera tout node lié par un même groupe pont) à 0
  int mark[g->nb_nodes];
  for(int i = 0; i < g->nb_nodes; i++)
    mark[i] = 0;
  
  //marque tout node lié par un même groupe de ponts
  recursive(g, 0, mark);
  for(int i = 0; i < g->nb_nodes; i++){
    printf("mark[%d] = %d\n", i, mark[i]);
    printf("d = %d, r_d = %d\n", get_degree(g, i), get_required_degree(game_node(g, i)));

    if(mark[i] != 1)
      return false;
    if(get_degree(g, i) != get_required_degree(game_node(g, i)))
      return false;
  }
  return true;
}

bool can_add_bridge_dir (cgame g, int node_num, dir d){

  if(get_neighbour_dir(g, node_num, d) == -1){
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
	  //ici, n est le node A
	  node n = game_node(g, i);
	  
	  if(get_x(n) > get_x(game_node(g, node_num)) //si xA > xD
	     && get_x(n) < get_x(game_node(g, get_neighbour_dir(g, node_num, NORTH))) //si xA < xC
	     && get_y(n) < get_y(game_node(g, node_num)) // si yA < yD (== yC)
	     && g->bridges[i][EAST] > 0) //si A a un pont vers l'est
	    return false;
	  
	  if(get_neighbour_dir(g, i, EAST) != -1 && //si A a un voisin B et ...
	     g->bridges[get_neighbour_dir(g, i, EAST)][WEST] > 0) //si B a un pont vers l'ouest
	     return false;
	}
	return true;
	break;
	
      case WEST:
	for(int i = 0; i < g->nb_nodes; i++){
	  //ici, n est le node D
	  node n = game_node(g, i);
	  
	  if(get_y(n) > get_y(game_node(g, get_neighbour_dir(g, node_num, WEST))) //si yD > yA
	     && get_y(n) < get_y(game_node(g, node_num)) //si yD < yB
	     && get_x(n) < get_x(game_node(g, node_num)) //si xD < xB (== xA)
	     && g->bridges[i][NORTH] > 0) //si D a un pont vers le nord
	    return false;
	  
	  if(get_neighbour_dir(g, i, NORTH) != -1 && //si D a un voisin C et ...
	     g->bridges[get_neighbour_dir(g, i, NORTH)][SOUTH] > 0) //si C a un pont vers le sud
	     return false;
	}
	return true;
	break;
	
      case SOUTH:
	for(int i = 0; i < g->nb_nodes; i++){
	  //ici, n est le node A
	  node n = game_node(g, i);
	  
	  if(get_x(n) < get_x(game_node(g, node_num)) //si xA < xC
	     && get_x(n) > get_x(game_node(g, get_neighbour_dir(g, node_num, SOUTH))) //si xA > xD
	     && get_y(n) < get_y(game_node(g, node_num)) // si yA < yC (== yD)
	     && g->bridges[i][EAST] > 0) //si A a un pont vers l'est
	    return false;
	  
	  if(get_neighbour_dir(g, i, EAST) != -1 && //si A a un voisin B et ...
	     g->bridges[get_neighbour_dir(g, i, EAST)][WEST] > 0) //si B a un pont vers l'ouest
	     return false;
	}
	return true;
	break;
	
      case EAST:
	for(int i = 0; i < g->nb_nodes; i++){
	  //ici, n est le node D
	  node n = game_node(g, i);
	  
	  if(get_y(n) < get_y(game_node(g, get_neighbour_dir(g, node_num, EAST))) //si yD < yB
	     && get_y(n) > get_y(game_node(g, node_num)) //si yD > yA
	     && get_x(n) > get_x(game_node(g, node_num)) //si xD > xA (== xB)
	     && g->bridges[i][NORTH] > 0) //si D a un pont vers le nord
	    return false;
	  
	  if(get_neighbour_dir(g, i, NORTH) != -1 && //si D a un voisin C et ...
	     g->bridges[get_neighbour_dir(g, i, NORTH)][SOUTH] > 0) //si C a un pont vers le sud
	     return false;
	}
	return true;
	break;
      }
  }
  return false;
}
