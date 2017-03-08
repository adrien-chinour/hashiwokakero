#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/file.h"

int get_nb_neighbours(game g, int num)
{
  //le compteur de voisins de num va incrémenter à chaque voisin vu
  int neighbours = 0;
  
  for(int i = 0; i < game_nb_dir(g); i++)
    {
      if(get_neighbour_dir(g, num, i) != -1)
	neighbours++;
    }
  return neighbours;
}

game evidence(game g){
  
  int node_num = 0;
  bool modif = true; int nb_modif = 0;
  int * tab = malloc(sizeof(int)*game_nb_dir(g));

  for(int i = 0; i < game_nb_nodes(g); i++) {
    float n = (float) get_required_degree(game_node(g, i))/game_nb_max_bridges(g);
    float m = (float) get_nb_neighbours(g, i);
    printf("%f\n",n);
    if(n >= m) {
      for(int j = 0; j < game_nb_dir(g); j++){
	if(can_add_bridge_dir(g, i, j)){
	  add_bridge_dir(g, i, j);
	}
      }
    }
    
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

game solver(game g, int argc, char *argv[]){
  g = evidence(g);
  //solver_recursive(g,0,0);
  return g; 
}

int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("UTILISATION: ./hashi_solve2 <nom_du_fichier> <nom_de_la_sauvegarde>\n");
    return EXIT_FAILURE;
  }
  
  game g = translate_game(argv[1]);

  g = solver(g,argc,argv);
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

  
    
