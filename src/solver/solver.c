#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/file.h"

bool solver_r(game g,int node_num,int dir,bool * go){
  //si le nombre de ponts max d'une ile et atteint
  if(get_required_degree(game_node(g,node_num))==get_degree(g,node_num)){
    //si on n'a pas parcouru chaque ile, on relance la récursivité avec la suivante
    if(node_num<(game_nb_nodes(g)-1)){
       solver_r(g,node_num+1,dir,go);
    }
    //mais si le jeu est fini, on donne le résultat
    else{
      if(game_over(g)){
         *go= true;
         return go;
      }
      //sinon quelque chose s'est mal passé, on supprime un pont
      else{
        if(dir>=0){
          del_bridge_dir(g,node_num,dir); //revenir en arrière

        }
      }
    } 
  }
  //si le degré de l'ile vue n'est pas atteint
  else{
    //on regarde dans toutes les directions
    for(int d=0; d<game_nb_dir(g);d++){
       if(can_add_bridge_dir(g,node_num,d)&&(!(get_required_degree(game_node(g,node_num))==get_degree(g,node_num)))&&(!(get_required_degree(game_node(g,get_neighbour_dir(g,node_num,d)))==get_degree(g,get_neighbour_dir(g,node_num,d))))){
	 //on peut poser un pont, on le pose et on relance la récursivité
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
  if( argc < 2){
    printf("UTILISATION ./hashi_solve <nom_du_fichier> <nom_de_sauvegarde>");
    return EXIT_FAILURE;
  }
  
  game g = translate_game(argv[1]);
  bool * go=malloc(sizeof(bool));
  *go=false;
  

  if(solver_r(g,0,-1,go)){
    printf("Solution trouvé!\n");
    if(game_over(g))
      printf("solution correct\n");
    else
      printf("Solution incorrect\n");
  }
  else {
    printf("Aucune solution.\n");
    return EXIT_SUCCESS;
  }
  
  char * save = malloc(sizeof(char)*100);
  if(argc > 2)
    write_save(g,argv[2]);
  else {
    sprintf(save, "%s.solved",argv[1]);
    write_save(g,save);
  }
  delete_game(g);
  free(save);
  return EXIT_SUCCESS;
}
