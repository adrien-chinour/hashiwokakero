#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/file.h"

/*
  Permet de compter le nombre de voisins d'une île où l'on peut poser un pont
*/
int get_nb_neighbours(game g, int num) {
  int neighbours = 0;
  for(int dir = 0; dir < game_nb_dir(g); dir++){
    if(get_neighbour_dir(g, num, dir) != -1)
      neighbours++;
  }
  return neighbours;
}

/*
  Permet d'ajouter les ponts obligatoires
  Résoud game_easy en 0.003s
*/
void simple_bridges(game g){
  for(int num = 0; num < game_nb_nodes(g); num++){
    //"rd" évite d'appeler 2 fois "get_required_degree"
    int rd = get_required_degree(game_node(g, num));
    //la variable "n" voit combien de pont(s) on peut poser vers les voisins du noeud
    int n = rd / game_nb_max_bridges(g) + rd % game_nb_max_bridges(g);
    
    //si le noeud n'a qu'un voisin, on lui pose forcément autant de pont que possible
    if(get_nb_neighbours(g, num) == 1){
      for(int dir = 0; dir < game_nb_dir(g); dir++){
	for(int i = 0; i < game_nb_max_bridges(g); i++)
	  add_bridge_dir(g, num, dir);
      }
    }
    
    //pour chaque voisin du noeud, si on peut poser un pont évident vers lui, on le pose
    else if(n == get_nb_neighbours(g, num)) {
      for(int dir = 0; dir < game_nb_dir(g); dir++){
	if(can_add_bridge_dir(g, num, dir))
	  add_bridge_dir(g, num, dir);
      }
    }
  }
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
