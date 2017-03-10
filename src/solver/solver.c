#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/file.h"

/*
  Permet de compter le nombre de voisins d'une île
*/
int get_nb_neighbours(game g, int num) {
  int neighbours = 0;
  for(int i = 0; i < game_nb_dir(g); i++){
      if(get_neighbour_dir(g, num, i) != -1) neighbours++;
  }
  return neighbours;
}

/*
  Permet d'ajouter les ponts obligatoires
  Résoud game_easy en 0.003s
*/
void simple_bridges(game g){
  
  int node_num = 0, nb_modif = 0;
  bool modif = true;
  int * tab = calloc(game_nb_dir(g), sizeof(int));

  for(int i = 0; i < game_nb_nodes(g); i++) {
    /* Initialisations des variables */
    int n = get_required_degree(game_node(g, i))/game_nb_max_bridges(g) + get_required_degree(game_node(g, i))%game_nb_max_bridges(g);
    
    /* Si le noeud a au minimum 1 connexion avec chacun de ses voisins */
    if(n == get_nb_neighbours(g, i)) {
      for(int j = 0; j < game_nb_dir(g); j++){
	if(can_add_bridge_dir(g, i, j)){
	  add_bridge_dir(g, i, j);
	}
      }
    }
    
    /* Si le noeud n'a qu'un voisin */
    else if(get_nb_neighbours(g, i) == 1){
      for(int j = 0; j < game_nb_dir(g); j++){
	for(int k = 0; k < game_nb_max_bridges(g); k++){
	  add_bridge_dir(g, i, j);
	}
      }
    }
  }
  
  while(modif){ 
    /* Initialisation des variables */
    int somme = 0;

    /* Boucle de test pour les solutions evidentes */
    for(int dir = 0; dir < game_nb_dir(g); dir++){

      /* pour eviter les appelles aux fonctions repetitifs */
      int degree = get_degree(g, node_num);
      int required_degree = get_required_degree(game_node(g, node_num));
      
      // si on peut ajouter des ponts
      if (can_add_bridge_dir(g,node_num,dir)){
	int degree_neighbour = get_degree(g, get_neighbour_dir(g, node_num, dir));
	int required_degree_neighbour = get_required_degree(game_node(g, get_neighbour_dir(g, node_num, dir)));
       
	// si le voisin a un degré inférieur au nombre de ponts max
	if(required_degree_neighbour < game_nb_max_bridges(g)) {
	  tab[dir] = required_degree_neighbour;
	  somme += required_degree_neighbour;
	}
	
	//si notre voisin peut compléter notre degré
	else if (degree < required_degree && required_degree - degree <= required_degree_neighbour - degree_neighbour) {
	  tab[dir] = required_degree - degree;
	  somme += required_degree - degree;
	  }
	else {
	  somme += game_nb_max_bridges(g);
	  tab[dir] = game_nb_max_bridges(g);
	}
      }
    }

    /* On ajoute les ponts obligatoires si il y en a*/
    if( somme > 0 && somme + get_degree(g, node_num) == get_required_degree(game_node(g, node_num)) ){
      for(int i = 0; i < game_nb_dir(g); i++){
	for(int j = 0; j < tab[i]; j++){
	  add_bridge_dir(g, node_num,i);
	}
      }
      nb_modif++;
    }

    /* test de fin de boucle while */
    if(node_num == game_nb_nodes(g)-1){
      if (nb_modif != 0){
	nb_modif = 0;
	node_num = -1;
      }
      else {
	modif = false;
      }
    }
      node_num++;
  }
  free(tab);
}

//si le nombre de ponts max d'une ile et atteint
bool solver_r(game g,int node_num,int dir,bool * go){
  
  if(get_required_degree(game_node(g,node_num))==get_degree(g,node_num)){
    if(node_num<(game_nb_nodes(g)-1)){
       solver_r(g,node_num+1,dir,go);
    }
    else{
      if(game_over(g)){
         *go= true;
         return go;
      }
      else{
         del_bridge_dir(g,node_num,dir);
      }
    }
  }
  else{
    for(int d=0; d<game_nb_dir(g);d++){
       if(can_add_bridge_dir(g,node_num,d)){
        add_bridge_dir(g,node_num,d);
        solver_r(g,node_num,d,go);
      }
    }
  }
  if(*go)
     return true;
  else
     del_bridge_dir(g, node_num, dir);
  return false;
}

int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("UTILISATION ./hashi_solve <nom_du_fichier> <nom_de_sauvegarde>");
    return EXIT_FAILURE;
  }
  
  game g = translate_game(argv[1]);
  simple_bridges(g);
  printf("ok");
  bool * go=malloc(sizeof(bool));
  *go=false;

  if(solver_r(g,0,-1,go))
     printf("Solution trouvé!\n");
  else 
     printf("Aucune solution.\n");

  char * save = malloc(sizeof(char)*100);
  if(argc > 2)
    write_save(g,argv[2]);
  else {
    sprintf(save, "%s.solved",argv[1]);
    write_save(g,save);
  }
  free(go);
  delete_game(g);
  free(save);
  return EXIT_SUCCESS;
}
