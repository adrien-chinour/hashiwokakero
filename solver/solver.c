#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../src/core/node.h"
#include "../src/core/game.h"
#include "../src/core/file.h"

//cette fonction va renvoyer un tableau contenant la solution du jeu
void solve(game g)
{
  //on regarde chaque node pour poser un pont
  for(int i = 0; i < game_nb_nodes(g); i++)
    {
      //on cherche à poser un pont dès que possible
      for(int j = 0; j < game_nb_dir(g); j++)
	{
	  //on voit les caractéristiques des voisins pour poser un pont
	  int neighbour = get_neighbour_dir(g, i, j), d_neighbour = get_degree(g, neighbour);
	  
	  //si un voisin est détecté, qu'il n'y a pas d'obstacle et que l'on peut poser un pont, on le fait
	  if(neighbour != -1 && can_add_bridge_dir(g, i, j) && d_neighbour != r_d_neighbour)
	    {
	      while(get_degree_dir(g, i, j) < game_nb_max_bridges)
		{
		  if(can_add_bridge_dir(g, i, j))
		    {
		      add_bridge_dir(g, i, j);
		      solve(g);
		    }
		}
	    }
	}
    }
}

int main(int argc, char *argv[])
{
  game g = translate_game(argv[1]);
  while(!game_over(g))
    {
      //on résout une grille donnée
      solve(g);
    }
  char * save;
  sprintf(save, "%s.solved",argv[1]);
  write_save(g,save);
  printf("solution found !\n");
  
  return EXIT_SUCCESS;
}
