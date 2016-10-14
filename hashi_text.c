#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

  //on change les paramètres en int (si ce n'est pas un nombre, ça vaudra 0)
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


//cette fonction demande à l'utilisateur sur quelle île placer un pont
void pont(int size, game jeu)
{
  int compte = 0;
  
  while(size > 0)
    {
      size = size/10;
      compte++;
    }
  
  char pose[compte+1], direction[2];
  
  printf("Veuillez saisir le numero de l'ile sur laquelle placer un pont\n");
  printf("les details des iles sont avant la grille:\n");
  
  //fgets va lire l'entrée de l'utilisateur avec le flux stdin (considéré comme un fichier)
  fgets(pose, sizeof(pose), stdin);
  
  int base = atoi(pose);
  
  fgets(direction, sizeof(direction), stdin);

  printf("Veuillez saisir un chiffre pour placer un pont autour de l'ile\n");
  printf("(NORD : 0 ; SUD : 1 ; EST : 2 ; OUEST : 3)\n");

  int dirige = atoi(pose);

  can_add_bridge_dir(jeu, base, dirige);
}


int main(int argc, char *argv[])
{
  //size sera la taille de la grille et nbnode le nombre de nodes
  int size, nbnode;

  //on vérifie si les réglages au lancement sont corrects
  securise(&(argc), argv, &(size), &(nbnode));
  
  //on crée un tableau 2D pour contenir toute la "grille" de jeu
  int grille[size][size];
  //on crée un tableau qui contient toutes les nodes
  node t[nbnode];

  //on crée la grille de départ
  initialise(size, nbnode, grille, t);
  
  //création d'une partie à partir des nodes précédement créés
  game jeu = new_game(nbnode, t);
  
  //vérification des nodes
  for(int i = 0; i < nbnode; i++)
    printf("ile numero %d (degre %d) : (%d; %d) \n", i, get_required_degree(t[i]), get_x(t[i]), get_y(t[i]));

  //on affiche la grille au départ
  affichage(size, grille);

  //on demande où placer le pont
  pont(size, jeu);
  
  return EXIT_SUCCESS;
}
