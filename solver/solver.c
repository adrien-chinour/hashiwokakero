#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "src/core/node.h"
#include "src/core/game.h"

//cette fonction va renvoyer un tableau contenant la solution du jeu
int solve(int num, cgame g)
{
  //on cherche à poser un pont dès que possible
  for(int i = 0; i < game_nb_dir(g); i++)
    {
      //on voit les caractéristiques des voisins pour poser un pont
      int voisin = get_neighbour_dir(g, num, i), d_voisin = get_degree(g, voisin);
      
      //si un voisin est détecté, qu'il n'y a pas d'obstacle et que l'on peut poser un pont, on le fait
      if(voisin != -1 && can_add_bridge_dir(g, num, i) && d_voisin != r_d_voisin)
	{
	  while(get_degree_dir(g, num, i) < game_nb_max_bridges)
	    {
	      if(can_add_bridge_dir(g, num, i))
		{
		  add_bridge_dir(g, num, i);
		  return solve(num, g);
		}
	    }
	}	 
    }
}

int main(int argc, char *argv[])
{
  int num = 0;
  
  while(!game_over(g))
    {
      //on résout une grille donnée
      num = solve(num, game);
    }
  printf("solution found !\n");
  
  return EXIT_SUCCESS;
}
