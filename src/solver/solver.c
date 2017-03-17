#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/file.h"


int get_nb_neighbours(game g, int num) {
  int neighbours = 0;
  for(int dir = 0; dir < game_nb_dir(g); dir++){
    if(get_neighbour_dir(g, num, dir) != -1)
      neighbours++;
  }
  return neighbours;
}

//cette fonction pose un les ponts évidents au cas où l'on peut poser 1 pont dans chaque direction d'un noeud
void bridges_in_all_directions(game g)
{
  for(int i = 0; i < game_nb_nodes(g); i++) {
    
    //cette variable n voit combien de pont on peut poser vers les voisins du noeud
    int n = get_required_degree(game_node(g, i))/game_nb_max_bridges(g)+get_required_degree(game_node(g, i))%game_nb_max_bridges(g);
    
    //pour chaque voisin du noeud, si on peut poser un pont évident vers lui, on le pose
    if(n == get_nb_neighbours(g, i)) {
      for(int j = 0; j < game_nb_dir(g); j++){
	if(can_add_bridge_dir(g, i, j)){
	  add_bridge_dir(g, i, j);
	}
      }
    }
    //si le noeud n'a qu'un voisin, on lui pose forcément autant de pont que possible
    else if(get_nb_neighbours(g, i) == 1){
      for(int j = 0; j < game_nb_dir(g); j++){
	for(int k = 0; k < game_nb_max_bridges(g); k++){
	  add_bridge_dir(g, i, j);
	}
      }
    }
  }
}


//ajoute les ponts obligatoires
void simple_bridges(game g){

  //node_num = n° de node ; nb_modif = quelque chose a été modifié ; modif = vérifie si on peut encore modifier
  int node_num = 0, nb_modif = 0;
  bool modif = true;
  int * tab = calloc(game_nb_dir(g), sizeof(int));

  bridges_in_all_directions(g);
  
  while(modif){ 
    // Initialisation des variables
    int somme = 0;

    // Boucle de test pour les solutions evidentes
    for(int dir = 0; dir < game_nb_dir(g); dir++){

      // pour eviter les appelles aux fonctions repetitifs
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

    // On ajoute les ponts obligatoires si il y en a
    if( somme > 0 && somme + get_degree(g, node_num) == get_required_degree(game_node(g, node_num)) ){
      for(int i = 0; i < game_nb_dir(g); i++){
	for(int j = 0; j < tab[i]; j++){
	  add_bridge_dir(g, node_num,i);
	}
      }
      nb_modif++;
    }

    // test de fin de boucle while
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

/*
int * calcul_bridges_neighbours(int node_num, game g){
  int * bridges = malloc(sizeof(int)*game_nb_dir(g) +1);
  bridges[game_nb_dir(g)] = 0;
  for(int dir = 0; dir < game_nb_dir(g); dir++){
    if(can_add_bridge_dir(g, node_num, dir)){
      
      //cas ou le degre est egal au degre du voisin
      if(get_required_degree(game_node(g, get_neighbour_dir(g, node_num, dir))) == get_required_degree(game_node(g, node_num)) && get_required_degree(game_node(g, node_num)) <= game_nb_max_bridges(g)){
	bridges[dir] = get_required_degree(game_node(g, node_num))-1;
	bridges[game_nb_dir(g)] += bridges[dir];
      }
      //cas ou notre noeud à un degré inférieur au degré max
      else if(get_required_degree(game_node(g, node_num)) - get_degree(g, node_num) < game_nb_max_bridges(g)){
	bridges[dir] = get_required_degree(game_node(g, node_num)) - get_degree(g, node_num);
	bridges[game_nb_dir(g)] += bridges[dir];
      }
      
      //cas ou le degré du voisin est inférieur au degré max 
      else if(get_required_degree(game_node(g, get_neighbour_dir(g, node_num, dir))) - get_degree(g, get_neighbour_dir(g, node_num, dir)) < game_nb_max_bridges(g)){
	bridges[dir] = get_required_degree(game_node(g, get_neighbour_dir(g, node_num, dir))) - get_degree(g, get_neighbour_dir(g, node_num, dir));
	bridges[game_nb_dir(g)] += bridges[dir];
      }
     
      //cas ou le degré du voisin est supérieur ou égal au degré max
      else {
	bridges[dir] = game_nb_max_bridges(g);
	bridges[game_nb_dir(g)] += bridges[dir];
      }
    }
    else
      bridges[dir] = 0;
  }
  return bridges;
}

void apply_bridges_neighbours(game g, int node_num, int * bridges){
  for(int dir = 0; dir < game_nb_dir(g); dir++){
    for(int i = 0; i < bridges[dir]; i++){
      add_bridge_dir(g, node_num, dir);
    }
  }
}


void auto_play(game g, int node_num, bool change){
  if(node_num == game_nb_nodes(g)){
    if (change)
      auto_play(g,0,false);
  }
  else {
    int * bridges = calcul_bridges_neighbours(node_num, g);
    if (bridges[game_nb_dir(g)] == get_required_degree(game_node(g, node_num)) - get_degree(g, node_num)){
      apply_bridges_neighbours(g, node_num, bridges);
      free(bridges);
      auto_play(g, node_num+1, true);
    }
    else{
      free(bridges);
      auto_play(g, node_num+1, change);
    }
  }
}
*/

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
  else {
    if(dir == -1)
      return false;
    del_bridge_dir(g, node_num, dir);
  }
  return false;
}

int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("UTILISATION ./hashi_solve <nom_du_fichier> <nom_de_sauvegarde>");
    return EXIT_FAILURE;
  }
  
  game g = translate_game(argv[1]);

  //auto_play(g, 0, false); //non fonctionnel
  simple_bridges(g);
  
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
