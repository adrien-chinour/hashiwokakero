#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "src/core/node.h"
#include "src/core/game.h"

//cette fonction va renvoyer un tableau contenant la solution du jeu
void solve(cgame g, int num)
{
  //on cherche à poser un pont dès que possible
  for(int i = 0; i < game_nb_dir(g); i++)
    {
      //on voit les caractéristiques des voisins pour poser un pont
      int neighbour = get_neighbour_dir(g, num, i), d_neighbour = get_degree(g, neighbour);
      
      //si un voisin est détecté, qu'il n'y a pas d'obstacle et que l'on peut poser un pont, on le fait
      if(neighbour != -1 && can_add_bridge_dir(g, num, i) && d_neighbour != r_d_neighbour)
	{
	  while(get_degree_dir(g, num, i) < game_nb_max_bridges)
	    {
	      if(can_add_bridge_dir(g, num, i))
		{
		  add_bridge_dir(g, num, i);
		  solve(g, num);
		}
	    }
	}	 
    }
}

int main(int argc, char *argv[])
{
  while(!game_over(g))
    {
      //on résout une grille donnée
      solve(game, 0);
    }
  printf("solution found !\n");
  
  return EXIT_SUCCESS;
}
