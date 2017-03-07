#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/file.h"

//cette fonction compte le nombre de voisins d'une île
int nb_neighbours(game g, int num)
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

void evidence(game g)
{
  //on parcours chaque node
  for(int i = 0; i < game_nb_nodes(g); i++)
    {
      //on regarde dans toutes les directions
      for(int j = 0; j < game_nb_dir(g); j++)
	{
	  //solution évidente 1: degré de l'île = nombre de voisins * nombre de ponts maximum
	  if(get_required_degree(game_node(g,i)) == nb_neighbours(g, i) * game_nb_max_bridges(g) && can_add_bridge_dir(g, i, j))
	    {
	      for(int k = 0; k < game_nb_max_bridges(g); k++)
	        add_bridge_dir(g, i, j);
	    }
	  //solution évidente 2: il n'y a qu'un voisin
	  else if(nb_neighbours(g, i) == 1)
	    {
	      for(int k = 0; k < game_nb_max_bridges(g); k++)
	        add_bridge_dir(g, i, j);
	    }
	  
	}
    }
}

bool solver_r(game g,int node_num,int dir){
  //si le nombre de ponts max d'une ile et atteint
  if(get_required_degree(game_node(g,node_num))==get_degree(g,node_num)){
    //si on n'a pas parcouru chaque ile, on relance la récursivité avec la suivante
    if(node_num<(game_nb_nodes(g)-1)){
      solver_r(g,node_num+1,dir);
    }
    //mais si le jeu est fini, on donne le résultat
    else{
      if(game_over(g)){
        return g;
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
        solver_r(g,node_num,d);
      }
    }
  }
}



int main(int argc, char *argv[]) {
  if( argc != 3){
    printf("Erreur nombre d'argument");
    return EXIT_FAILURE;
  }
  
  game g = translate_game(argv[1]);

  if(solver_r(g,0,-1))
    printf("solution found !\n");
  else
    printf("solution not found !\n");
  if(game_over(g))
    printf("ok\n");
  else
    printf("not ok\n");
  
  char * save = malloc(sizeof(char)*100);
  sprintf(save, "%s.solved",argv[1]);
  write_save(g,save);
  //printf("solution found !\n");
  free(save);
  return EXIT_SUCCESS;
}
