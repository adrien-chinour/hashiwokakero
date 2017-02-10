#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../include/node.h"
#include "../include/game.h"

//cette fonction va créer des tableaux pour le jeu
void solve(cgame g)
{
  //on cherche à placer aléatoirement des ponts sur chaque node
  for(int i = 0; i < game_nb_nodes(g); i++)
    {
      for(int j = 0; j < game_nb_dir(g); j++)
	{
	  //si un voisin est détecté et qu'il n'y a pas d'obstacle on pose un pont
	  if(get_neighbour_dir(g, i, j) != -1 && can_add_bridge_dir(g, i, j))
	    add_bridge_dir(g, i, j);
	  
	  if(game_over(g))
	    printf("solution found !\n");
	}
    }
}

int main(int argc, char *argv[])
{ 
  //on résout une grille donnée
  solve(game);
  
  return EXIT_SUCCESS;
}
