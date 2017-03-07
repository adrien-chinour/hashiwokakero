#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/file.h"

game evidence(game g){
  
  int node_num = 0;
  bool modif = true; int nb_modif = 0;
  int * tab = malloc(sizeof(int)*game_nb_dir(g));
  
  while(modif){
    /* Initialisation des variables */
    int somme = 0;
    for(int i = 0; i < game_nb_dir(g); i++){
      tab[i] = 0;
    }

    /* Boucle de test pour les solutions evidentes */
    for(int dir = 0; dir < game_nb_dir(g); dir++){

      /* pour eviter les appelles aux fonctions repetitif */
      int degree = get_degree(g, node_num);
      int required_degree = get_required_degree(game_node(g, node_num));
      
      // si on peut ajouter des ponts
      if (degree != required_degree
	  && can_add_bridge_dir(g,node_num,dir)
	  &&  get_degree(g, get_neighbour_dir(g, node_num, dir)) != get_required_degree(game_node(g, get_neighbour_dir(g, node_num, dir)))){
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
    if( somme > 0 && somme + get_degree(g, node_num) == get_required_degree(game_node(g, node_num)) ){
      for(int i = 0; i < game_nb_dir(g); i++){
	for(int j = 0; j < tab[i]; j++){
	  add_bridge_dir(g, node_num,i);
	}
      }
      
      nb_modif++;
      printf("%d,%d,%d\n",node_num,somme,nb_modif);
    }
    if(node_num == game_nb_nodes(g)-1){
      if (nb_modif != 0){
	nb_modif = 0;
	node_num = 0;
      }
      else {
	modif = false;
      }
    }
   
    node_num++;
  }
  free(tab);
  return g;
}

game solver_recursive(game g, int node_num, int dir, bool recul){
  if (game_over(g))
    return g;
  else if (!recul && can_add_bridge_dir(g, node_num, dir) ){
    if(get_degree(g, node_num) < get_required_degree(game_node(g, node_num))){
      while(can_add_bridge_dir(g, node_num, dir)){
	add_bridge_dir(g, node_num, dir);
      }
      return solver_recursive(g,node_num,dir+1,false);
    }
    else
      return solver_recursive(g,node_num+1,0,false);
  }
  else if (recul && can_add_bridge_dir(g, node_num, dir) ){
    int other_dir = dir-1;
    while(!can_add_bridge_dir(g,node_num,other_dir)){
      if(other_dir == 0)
	other_dir = game_nb_dir(g);
      other_dir--;
    }
    del_bridge_dir(g, node_num, other_dir);
    add_bridge_dir(g, node_num, dir);
    return solver_recursive(g, node_num+1, 0, false);
  }
  else if (dir != game_nb_dir(g)-1)
    return solver_recursive(g, node_num, dir+1, false);
  else
    return solver_recursive(g, node_num-1, 0, true);
}

game solver(game g){
  g = evidence(g);
  //g = solver_recursive(g,0,0,false);
  return g; 
}

int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("UTILISATION: ./hashi_solve2 <nom_du_fichier> <nom_de_la_sauvegarde>\n");
    return EXIT_FAILURE;
  }
  
  game g = translate_game(argv[1]);

  g = solver(g);
  printf("Solution trouvé!\n");
  
  char * save = malloc(sizeof(char)*100);
  if(save == NULL) fprintf(stderr, "Problème d'allocation du nom du fichier de sortie.\n");
  
  if(argc > 2){
    printf("solution sauvegardée dans le fichier %s\n",argv[2]);
    write_save(g,argv[2]);
  }
  else {
    sprintf(save, "%s.solved",argv[1]);
    printf("solution sauvegardée dans le fichier %s\n",save);
    write_save(g,save);
  }

  delete_game(g);
  free(save);
  return EXIT_SUCCESS;
}

  
    
