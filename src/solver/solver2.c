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
  bool modif = true;
  int nb_modif = 0;
  int * tab = malloc(sizeof(int)*game_nb_dir(g));
  while(modif){
    int somme = 0;
    for(int i = 0; i < game_nb_dir(g); i++){
      tab[i] = 0;
    }
    for(int dir = 0; dir < game_nb_dir(g); dir++){
      // si on peut ajouter des ponts
      if (get_degree(g, node_num) != get_required_degree(game_node(g, node_num)) && can_add_bridge_dir(g,node_num,dir) && get_degree(g, get_neighbour_dir(g, node_num, dir)) != get_required_degree(game_node(g, get_neighbour_dir(g, node_num, dir)))){
	// si le voisin a un degré inférieur au nombre de ponts max
	if(get_required_degree(game_node(g, get_neighbour_dir(g, node_num, dir))) < game_nb_max_bridges(g)) {
	  somme += get_required_degree(game_node(g, get_neighbour_dir(g, node_num, dir)));
	  tab[dir] =  get_required_degree(game_node(g, get_neighbour_dir(g, node_num, dir)));
	}
	//si notre voisin peut compléter notre degré
	else if (get_degree(g, node_num) < get_required_degree(game_node(g, node_num))
		 && get_required_degree(game_node(g, node_num))-get_degree(g, node_num) <= get_required_degree(game_node(g, get_neighbour_dir(g, node_num, dir)))-get_degree(g, get_neighbour_dir(g, node_num, dir))) {
	  tab[dir] = get_required_degree(game_node(g, node_num))-get_degree(g, node_num);
	  somme += get_required_degree(game_node(g, node_num))-get_degree(g, node_num);
	  }
	else {
	  somme += game_nb_max_bridges(g);
	  tab[dir] = game_nb_max_bridges(g);
	}
      }
    }
    if( somme > 0 && somme + get_degree(g,node_num) == get_required_degree(game_node(g, node_num))){
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

game solver(game g){
  g = evidence(g);
  if(game_over(g)){
    return g;
  }
  else
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
  if(argc > 2){
    printf("solution sauvegardée dans le fichier %s\n",argv[2]);
    write_save(g,argv[2]);
  }
  else {
    sprintf(save, "%s.solved",argv[1]);
    printf("solution sauvegardée dans le fichier %s\n",save);
    write_save(g,save);
  }
  
  free(save);
  return EXIT_SUCCESS;
}

  
    
