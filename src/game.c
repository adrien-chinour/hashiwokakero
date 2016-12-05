#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/node.h"

typedef enum dir_e {NORTH, WEST, SOUTH, EAST, NW, SW, SE, NE} dir;

typedef struct game_s {
  int nb_nodes;
  node *nodes; 
  int **bridges;
  int nb_dir;
  int nb_max_bridges;
} *game;

typedef const struct game_s* cgame;

game new_game (int nb_nodes, node *nodes, int nb_max_bridges, int nb_dir){
  
  game g = (game) malloc(sizeof(struct game_s));
	
  g->nb_dir = nb_dir;
  g->nb_max_bridges = nb_max_bridges;
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
		bridges[i] = (int*) malloc(nb_dir*sizeof(int)); 
	}

	for(int i = 0; i < nb_nodes; i++){
		for(int j = 0; j < nb_dir; j++){
			bridges[i][j] = 0;
		}
	}

	g->bridges = bridges;

	return g;
}

int game_nb_nodes (cgame g){
   return g->nb_nodes;
}

int game_nb_dir (cgame g){
  return g->nb_dir;
}

int game_nb_max_bridges (cgame g){
  return g->nb_max_bridges;
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

	game g = (game) malloc(sizeof(struct game_s));
	
	g->nb_nodes = g_src->nb_nodes;
  g->nb_dir = g_src->nb_dir;
  g->nb_max_bridges = g_src->nb_max_bridges;

	
  // allocation et initialisation de nodes[nb_nodes]
	node *new_nodes = (node*) malloc(game_nb_nodes(g)*sizeof(node));
  for(int i = 0; i < game_nb_nodes(g); i++){
    new_nodes[i] = new_node(get_x(g_src->nodes[i]), get_y(g_src->nodes[i]), get_required_degree(g_src->nodes[i]));
  }
	g->nodes = new_nodes;

  // allocation et initialisation de bridges[nb_nodes][nb_dir]
  int **bridges;
  bridges = malloc(game_nb_nodes(g)*sizeof(int*)); 
  for(int i = 0; i < game_nb_nodes(g); i++){
    bridges[i] = malloc(game_nb_dir(g)*sizeof(int)); 
  }
	for(int i = 0; i < game_nb_nodes(g); i++){
		for(int j = 0; j < game_nb_dir(g); j++){
			bridges[i][j] = g_src->bridges[i][j];
		}
	}

	g->bridges = bridges;

	return g;
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

int get_degree_dir (cgame g, int node_num, dir d){
   return g->bridges[node_num][d];
}

int get_degree(cgame g, int node_num){
   int k = 0;
   for(int i = 0; i < game_nb_dir(g); i++){
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
      
      while (y <= max_y && game_get_node_number(g,get_x(n),y) == -1){
        y++;  
      }
      if(game_get_node_number(g,get_x(n),y) != -1)
        return game_get_node_number(g,get_x(n),y);
      break;
    }
    
  case NE:
    {
      int max_y = 0, max_x = 0;
      
      for(int i = 0; i < g->nb_nodes; i++){
        if(get_y(g->nodes[i]) > max_y)
          max_y = get_y(g->nodes[i]);
	if(get_x(g->nodes[i]) > max_x)
          max_x = get_x(g->nodes[i]);
      }
      
      int y = get_y(n)+1, x = get_x(n)+1;
      
      while (x <= max_x && y <= max_y && game_get_node_number(g,x,y) == -1){
	x++;
	y++;  
      }
      if(game_get_node_number(g,x,y) != -1)
        return game_get_node_number(g,x,y);
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
      
      while (x <= max_x && game_get_node_number(g,x,get_y(n)) == -1){
        x++;
      }
      if(game_get_node_number(g,x,get_y(n)) != -1)
        return game_get_node_number(g,x,get_y(n));
      break; 
    }

  case SE:
    {
      int max_x = 0;
      
      for(int i = 0; i < g->nb_nodes; i++){
	if(get_x(g->nodes[i]) > max_x)
          max_x = get_x(g->nodes[i]);
      }
      
      int x = get_x(n)+1, y = get_y(n)-1;
      
      while (x <= max_x && y >= 0 && game_get_node_number(g,x,y) == -1){
	x++;
	y--;  
      }
      if(game_get_node_number(g,x,y) != -1)
        return game_get_node_number(g,x,y);
      break;
    }

  case SOUTH:
    {
      int y = get_y(n)-1;
      
      while(y >= 0 && game_get_node_number(g,get_x(n),y) == -1){
        y--;
      }
      if(game_get_node_number(g,get_x(n),y) != -1)
        return game_get_node_number(g,get_x(n),y);
      break;
    }

  case SW:
    {
      int x = get_x(n)-1, y = get_y(n)-1;
      
      while (x >= 0 && y >= 0 && game_get_node_number(g,x,y) == -1){
	x--;
	y--;  
      }
      if(game_get_node_number(g,x,y) != -1)
        return game_get_node_number(g,x,y);
      break;
    }

  case WEST:
    {
      int x = get_x(n)-1;
      
      while(x >= 0 && game_get_node_number(g,x,get_y(n)) == -1){
        x--;
      }
      if(game_get_node_number(g,x,get_y(n)) != -1)
        return game_get_node_number(g,x,get_y(n));
      break;
    }
    
  case NW:
    {
      int max_y = 0;
      
      for(int i = 0; i < g->nb_nodes; i++){
	if(get_y(g->nodes[i]) > max_y)
          max_y = get_y(g->nodes[i]);
      }
      
      int x = get_x(n)+1, y = get_y(n)-1;
      
      while (x >= 0 && y <= max_y && game_get_node_number(g,x,y) == -1){
	x--;
	y++;  
      }
      if(game_get_node_number(g,x,y) != -1)
        return game_get_node_number(g,x,y);
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


 bool game_over (cgame g){
 
  // Fonction déclarée a l'intérieur car elle n'est utilisée que par game_over
    void explore(cgame g, int node_num, bool connected[]){
    
    //initialise un tableau qui est a true si le node est lié aux autres
    connected[node_num] = true;
    
    if(get_degree_dir(g, node_num, SOUTH) != 0){
      if(connected[get_neighbour_dir(g, node_num, SOUTH)] == false)
        explore(g, get_neighbour_dir(g, node_num, SOUTH), connected);
    }
    
    if(get_degree_dir(g, node_num, NORTH) != 0){
      if(connected[get_neighbour_dir(g, node_num, NORTH)] == false)
        explore(g, get_neighbour_dir(g, node_num, NORTH), connected);
    }
    
    if(get_degree_dir(g, node_num, EAST) != 0){
      if(connected[get_neighbour_dir(g, node_num, EAST)] == false)
        explore(g, get_neighbour_dir(g, node_num, EAST), connected);
    }
    
    if(get_degree_dir(g, node_num, WEST) != 0){
      if(connected[get_neighbour_dir(g, node_num, WEST)] == false)
        explore(g, get_neighbour_dir(g, node_num, WEST), connected);
    }

     if(get_degree_dir(g, node_num, NW) != 0){
       if(connected[get_neighbour_dir(g, node_num, NW)] == false)
          explore(g, get_neighbour_dir(g, node_num, NW), connected);
     }

     if(get_degree_dir(g, node_num, SW) != 0){
        if(connected[get_neighbour_dir(g, node_num, SW)] == false)
           explore(g, get_neighbour_dir(g, node_num, SW), connected);
     }

     if(get_degree_dir(g, node_num, SE) != 0){
        if(connected[get_neighbour_dir(g, node_num, SE)] == false)
           explore(g, get_neighbour_dir(g, node_num, SE), connected);
     }

     if(get_degree_dir(g, node_num, NE) != 0){
        if(connected[get_neighbour_dir(g, node_num, NE)] == false)
           explore(g, get_neighbour_dir(g, node_num, NE), connected);
     }
     
    }

  //verification des degrées
  for(int i = 0; i < game_nb_nodes(g); i++){
    if(get_degree(g, i) != get_required_degree(game_node(g, i)))
      return false;
  }

  // verification de la connexité
  bool connected[game_nb_nodes(g)];
  for(int i = 0; i < game_nb_nodes(g); i++){
    connected[i] = false;
  } 

  explore(g, 0, connected);
  
  for(int i = 0; i < game_nb_nodes(g); i++){
    if(connected[i] == false)
      return false;
  } 

  return true;
}

bool can_add_bridge_dir (cgame g, int node_num, dir d){

  if(get_neighbour_dir(g, node_num, d) == -1){
    return false;
  }

  if(g->bridges[node_num][d] <= game_nb_max_bridges(g)){

    node n0 = game_node(g, node_num), n1 = game_node(g, get_neighbour_dir(g, node_num, d));
    int x0 = get_x(n0), y0 = get_y(n0), x1 = get_x(n1), y1 = get_y(n1);
    double a0 = (y0-y1)/(x0-x1), b0 = y0-a0*x0;

    if(x0 > x1)
      {
	int change = x0;
	x0 = x1;
	x1 = change;
      }
    
    if(y0 > y1)
      {
	int change = y0;
	y0 = y1;
	y1 = change;
      }

    for(int i = 0; i < g->nb_nodes; i++)
      {
	for(int j = 0; j < g->nb_dir; j++)
	  {
	    node n = game_node(g, i), nv = game_node(g, get_neighbour_dir(g, i, j));
	    int xn = get_x(n), yn = get_y(n), xv = get_x(nv), yv = get_y(nv);
	    double ai = (yn-yv)/(xn-xv), bi = (yn-ai*xn);
	    
	    if(a0 != ai)
	      {
		//coordonnées du point d'intersection des droites
		double x = (bi-b0)/(a0-ai), y = ai*xn+bi;

		//si (x;y) est sur le pont à poser et si (x;y) est sur un autre pont
		if(x0 < x && x < x1 && y0 < y && y < y1 && xn < x && x < xv && yn < y && y < yv)
		  return false;
	      }
	  }


    /*
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

      case NW:
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

      case SW:
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

      case SE:
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

      case NE:
	break;
    */
      }
    return true;
  }
  return false;
}
