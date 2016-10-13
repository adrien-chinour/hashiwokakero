#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "node.h"
#include "game.h"

//cette fonction vérifie si l'utilisateur lance le programme correctement
void securise(int *argc, char *argv[], int *size, int *nbnode)
{
  /*lors du lancement, l'utilisateur doit choisir:
    > la taille de la grille de jeu
    > le nombre de nodes dans la grille
  sinon le jeu ne peut pas commencer*/
  
  if(*argc != 3)
    {
      printf("Erreur dans le nombre d'argument.\nSynthaxe : ./hashi_text TAILLE NBNODE\n");
      exit(EXIT_FAILURE);
    }

  //en change les paramètres en int (si ce n'est pas un nombre, ça vaudra 0)
  *size = atoi(argv[1]);
  *nbnode = atoi(argv[2]);

  //si l'utilisateur écrit autre chose que des nombres, ça ne marche pas
  if(*size == 0 || *nbnode == 0 )
  {
    printf("Erreur : veuillez entrer 2 nombres !\n");
    printf("Synthaxe : ./hashi_text TAILLE NBNODE\n");
    exit(EXIT_FAILURE);
  }
  
  //s'il y a plus de nodes que de "cases" dans la grille, le jeu ne doit pas se lancer
  if((*size)*(*size) < *nbnode)
    {
      printf("Erreur : la grille de jeu ne peut pas contenir plus de nodes que de cases !\n");
      printf("Synthaxe : ./hashi_text TAILLE NBNODE\n");
      exit(EXIT_FAILURE);
    }
}


//cette fonction affiche l'état actuel du jeu
void affichage(int size, int grille[size][size])
{
  for(int i = 0; i < size; i++)
    {
      if(i < 10)
	printf(" %d  ", i);
      else
	printf(" %d ", i);
    }
  printf("\n");
  for(int i = 0; i < size; i++)
    {
      printf("----");
    }
  printf("\n");
  
  for(int y = size-1; y >= 0; y--)
    {
      for(int x = 0; x < size; x++)
	{
	  if(grille[x][y] == -1)
	    printf(" .  ");
	  if(grille[x][y] > 0)
	    printf(" %d  ", grille[x][y]);
	  if(grille[x][y] == 0)
	    printf(" #  ");
	}
      printf("| %d\n\n", y);
    }
  printf("-- fin de la zone de jeu --\n");
}


//cette fonction va créer des tableaux pour le jeu
void initialise(int size, int nbnode, int grille[size][size], node t[])
{
  for(int i = 0; i < size; i++)
    {
      for(int j = 0; j < size; j++)
	{
	  grille[i][j] = -1;
	}
    }
  
  srand(time(NULL));
  
  for(int i = 0; i < nbnode; i++){
    int r_x = rand()% size;
    int r_y = rand()% size;
    int r_d = rand()% 8 + 1;
    t[i] = new_node(r_x, r_y, r_d);
    grille[r_x][r_y] = r_d;
  }
}

int main(int argc, char *argv[])
{
  //size sera la taille de la grille et nbnode le nombre de nodes
  int size, nbnode;
  
  securise(&(argc), argv, &(size), &(nbnode));
  
  //on crée un tableau 2D pour contenir toute la "grille" de jeu
  int grille[size][size];
  //on crée un tableau qui contient toutes les nodes
  node t[nbnode];
  
  initialise(size, nbnode, grille, t);
  
  /*création d'une partie à partir des nodes précédement créés */
  new_game(nbnode, t);
  
  /*vérification de 2 nodes*/
  printf("t[0]=%d, %d, %d\n", get_x(t[0]), get_y(t[0]), get_required_degree(t[0]));
  printf("t[1]=%d, %d, %d\n\n", get_x(t[1]), get_y(t[1]), get_required_degree(t[1]));

  affichage(size, grille);
  
  return EXIT_SUCCESS;
}
