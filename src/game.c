#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
  assert(g != NULL);
	
  g->nb_dir = nb_dir;
  g->nb_max_bridges = nb_max_bridges;
  g->nb_nodes = nb_nodes;
	
  node *new_nodes = (node*) malloc(nb_nodes*sizeof(node));
  assert(new_nodes != NULL);
  for(int i = 0; i < nb_nodes; i++){
    new_nodes[i] = new_node(get_x(nodes[i]), get_y(nodes[i]), get_required_degree(nodes[i]));
  }
	g->nodes = new_nodes;

	// http://www.commentcamarche.net/forum/affich-4931713-tableau-dynamique-a-2-dimensions
	int **bridges;
	bridges = (int**) malloc(nb_nodes*sizeof(int*)); 
  assert( bridges != NULL);
	for(int i = 0; i < nb_nodes; i++){
		bridges[i] = (int*) malloc(nb_dir*sizeof(int)); 
    assert( bridges[i] != NULL);
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
  assert(g != NULL);
	
	g->nb_nodes = g_src->nb_nodes;
  g->nb_dir = g_src->nb_dir;
  g->nb_max_bridges = g_src->nb_max_bridges;

	
  // allocation et initialisation de nodes[nb_nodes]
	node *new_nodes = (node*) malloc(game_nb_nodes(g)*sizeof(node));
  assert(new_nodes != NULL);
  for(int i = 0; i < game_nb_nodes(g); i++){
    new_nodes[i] = new_node(get_x(g_src->nodes[i]), get_y(g_src->nodes[i]), get_required_degree(g_src->nodes[i]));
  }
	g->nodes = new_nodes;

  // allocation et initialisation de bridges[nb_nodes][nb_dir]
  int **bridges;
  bridges = malloc(game_nb_nodes(g)*sizeof(int*));
  assert(bridges != NULL); 
  for(int i = 0; i < game_nb_nodes(g); i++){
    bridges[i] = malloc(game_nb_dir(g)*sizeof(int)); 
    assert(bridges[i] != NULL);
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


static int inverse_dir(dir d){
   int back;
   switch(d)
   {
      case NORTH:
         back = SOUTH;
         break;
	case SOUTH:
           back = NORTH;
           break;
      case EAST:
         back = WEST;
         break;
      case WEST:
         back = EAST;
	break;
      case NW:
         back = SE;
         break;
      case SW:
         back = NE;
         break;
      case SE:
         back = NW;
         break;
	case NE:
           back = SW;
           break;
   }
   return back;
}

void add_bridge_dir (game g, int node_num, dir d){
  if(can_add_bridge_dir (g, node_num, d)){
    g->bridges[node_num][d]++;
    g->bridges[get_neighbour_dir(g, node_num, d)][inverse_dir(d)]++;
  }
  else
      printf("Erreur: can_add_bridge_dir (g, node_num, d) n'est pas valide.\n");
}

void del_bridge_dir (game g, int node_num, dir d){
   if(g->bridges[node_num][d] != 0){
      g->bridges[get_neighbour_dir(g,node_num,d)][inverse_dir(d)]--;
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
      int max_x = 0, max_y = 0;
      
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
      
      int x = get_x(n)-1, y = get_y(n)+1;
      
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

static void explore(cgame g, int node_num, bool connected[]){
   
  //initialise un tableau qui est a true si le node est lié aux autres
   
  connected[node_num] = true;
   
  for(int i = 0; i < game_nb_dir(g); i++){
    if(get_degree_dir(g, node_num, i) != 0){
      if(connected[get_neighbour_dir(g, node_num, i)] == false)
         explore(g, get_neighbour_dir(g, node_num, i), connected);
    }
  }   
}


bool game_over (cgame g){
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

  //si l'ile n'a pas de voisin, on ne peut pas poser de pont
  if(get_neighbour_dir(g, node_num, d) == -1) return false;
    
  //si le noeud a atteint le nombre maximum de ponts
  //if(get_degree(g, node_num) >= get_required_degree(game_node(g, node_num))) return false;

  //si le voisin a atteint le nombre max de pont
  //if(get_degree(g,get_neighbour_dir(g, node_num, d)) >= get_required_degree(game_node(g, get_neighbour_dir(g, node_num, d)))) return false;

  //si le noeud a atteint le maximum de pont dans une direction
  if(get_degree_dir(g, node_num, d) >= game_nb_max_bridges(g)) return false;

  //croisement
  int x1 = get_x(game_node(g, get_neighbour_dir(g, node_num, d))) - get_x(game_node(g, node_num)); //AB x
  int y1 = get_y(game_node(g, get_neighbour_dir(g, node_num, d))) - get_y(game_node(g, node_num)); //AB y

  for(int i = 0; i < game_nb_nodes(g); i++){
    if(i != node_num && i != get_neighbour_dir(g, node_num, d)){ // on ne regarde pas le node node_num ni son voisin dans la direction
      for(int j = 0; j < game_nb_dir(g); j++){
        if(get_degree_dir(g, i, j) != 0 && j != d && j != inverse_dir(d)){ // on ne regarde pas les directions parallèles
          //coordonnées des vecteurs utiles
          int x2 = get_x(game_node(g, get_neighbour_dir(g, i , j))) - get_x(game_node(g, i)); //CD x
          int y2 = get_y(game_node(g, get_neighbour_dir(g, i , j))) - get_y(game_node(g, i)); //CD y
          int x3 = get_x(game_node(g, i)) - get_x(game_node(g, node_num)); //AC x
          int y3 = get_y(game_node(g, i)) - get_y(game_node(g, node_num)); //AC y
          int x4 = get_x(game_node(g, get_neighbour_dir(g, i, j))) - get_x(game_node(g, node_num)); //AD x
          int y4 = get_y(game_node(g, get_neighbour_dir(g, i, j))) - get_y(game_node(g, node_num)); //AD y
          int x5 = get_x(game_node(g, get_neighbour_dir(g, node_num, d))) - get_x(game_node(g, i)); //CB x
          int y5 = get_y(game_node(g, get_neighbour_dir(g, node_num, d))) - get_y(game_node(g, i)); //CB y
          int x6 = get_x(game_node(g, node_num)) - get_x(game_node(g, i)); //CA x
          int y6 = get_y(game_node(g, node_num)) - get_y(game_node(g, i)); //CA y

          //printf("test intersection CD : %d;test intersection AB : %d;node_num = %d\n", ( x1*y3 - y1*x3 ) * ( x1*y4 - y1*x4 ),   (x2*y5 - y2*x5 ) * ( x2*y6 - y2*x6 ), node_num);
          if( ( x1*y3 - y1*x3 ) * ( x1*y4 - y1*x4 ) < 0 && ( x2*y5 - y2*x5 ) * ( x2*y6 - y2*x6 ) < 0 ) return false; // Est-ce que le point d'intersection est sur [CD] et [AB]  ?
        }
      }
    }
  }

  /*  Intersection entre [AB] et [CD] ?
    Vrai ssi  vectoriel(vecteur(AB),vecteur(CD)) != 0  => signifie qu'ils ne sont pas // 
              && vectoriel(vecteur(AB),vecteur(AD)) * vectoriel(vecteur(AB),vecteur(AC)) < 0 => signifie que les deux vecteurs se croisent sur [CD]
    Le premier test est effectué dans le deuxième if et le deuxieme dans le troisième if (phrase nul a chier)
  */

  /*il manque un test par rapport au nombre de ponts aussi */
  return true;
}
