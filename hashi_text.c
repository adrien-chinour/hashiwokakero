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
    > la taille de la grille de jeu (0 à 49)
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

  //s'il y a moins d'une node ou plus de nodes que de "cases", le jeu ne doit pas se lancer
  if((*size)*(*size) < *nbnode || *nbnode < 0)
    {
      *size = 0;
    }
  
  //si l'utilisateur écrit autre chose que des nombres corrects, ça ne marche pas
  if(*size == 0 || *nbnode == 0)
  {
    printf("Erreur : veuillez entrer la taille entre 1 et 50 et les nodes entre 1 et 250 !\n");
    printf("Synthaxe : ./hashi_text TAILLE NBNODE\n");
    exit(EXIT_FAILURE);
  }

  //si la taille du jeu est hors normes (taille < 0 ou taille > 50), on indique le problème
  if(*size < 0 || *size > 50)
    {
      printf("Erreur : la grille de jeu choisie ne peut pas etre construite (choisissez de 1 à 50) !\n");
      printf("Synthaxe : ./hashi_text TAILLE NBNODE\n");
      exit(EXIT_FAILURE);
    }
}


//cette fonction affiche l'état actuel du jeu
void affichage(int size, int grille[size][size], game jeu)
{
  //affiche l'axe des abscisses
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

  //affiche toute la grille
  for(int y = size-1; y >= 0; y--)
    {
      for(int x = 0; x < size; x++)
	{
	  //affiche une case vide
	  if(grille[x][y] == -1)
	    printf(" .  ");
	  
	  //affiche un pont
	  if(grille[x][y] == -2)
	    printf(" +  ");
	  
	  //affiche le degré d'une île
	  if(grille[x][y] >= 0)
	    printf(" %d  ", grille[x][y]);
	}
      //affiche l'axe des ordonnées
      printf("| %d\n\n", y);
    }
  printf("-- fin de la zone de jeu --\n");
}


//cette fonction va créer des tableaux pour le jeu
game initialise(int size, int nbnode, int grille[size][size], node t[])
{
  //on initialise la grille à -1 (case vide)
  for(int i = 0; i < size; i++)
    {
      for(int j = 0; j < size; j++)
	{
	  grille[i][j] = -1;
	}
    }
  
  //on crée des nodes aléatoires
  srand(time(NULL));

  int x = rand()%size, y = rand()%size;
  t[0] = new_node(x, y, 0);
  grille[x][y] = 0;
  
  for(int i = 1; i < nbnode; i++)
    {
      int randX = x, randY = y;

      while(grille[randX][randY] == 0)
	{
	  int pileFace = rand()%2;
	  
	  if(pileFace == 0)
	    randX = rand()%size;
	  else
	    randY = rand()%size;
	}
      x = randX;
      y = randY;
      t[i] = new_node(x, y, 0);
      grille[x][y] = 0;
    }

  //création du jeu à partir des nodes précédement créées
  game jeu = new_game(nbnode, t);

  for(int i = 0; i < nbnode; i++)
    {
      while(get_degree(jeu, i) == 0)
	{
	  for(int j = 0; j < NB_DIRS; j++)
	    {
	      int voisin = get_neighbour_dir(jeu, i, j), pileFace = rand()%2;

	      //si un voisin est détecté et qu'il n'y a pas d'obstacle on pose un pont ou pas
	      if(voisin != -1 && pileFace == 0  && can_add_bridge_dir(jeu, i, j))
		{
		  add_bridge_dir(jeu, i, j);
		  /*
		  int x = get_x(t[i]), y = get_y(t[i]);
		  
		  //on affiche les ponts
		  if(j == NORTH)
		    {
		      while(y != get_y(t[voisin])-1)
			{
			  y++;
			  grille[x][y] = -2;
			}
		    }
		  else if(j == WEST)
		    {
		      while(x != get_x(t[voisin])+1)
			{
			  x--;
			  grille[x][y] = -2;
			}
		    }
		  else if(j == SOUTH)
		    {
		      while(y != get_y(t[voisin])+1)
			{
			  y--;
			  grille[x][y] = -2;
			}
		    }
		  else
		    {
		      while(y != get_y(t[voisin])-1)
			{
			  y++;
			  grille[x][y] = -2;
			}
		    }
		  */
		}
	    }
	}
    }
  return jeu;
}


//le buffer est comme une mémoire qui prend les saisies, il faut le vider en cas de problème
void lecture_securisee(char chaine[], int longueur)
{
  //on cherche où l'utilisateur a tapé '\n' avec la touche "entrer"
  char *positionEntree = NULL;

  //fgets va lire l'entrée de l'utilisateur avec le flux stdin (considéré comme un fichier)
  if(fgets(chaine, longueur, stdin) != NULL)
    {
      //strchr cherche le caractère '\n' dans chaine
      positionEntree = strchr(chaine, '\n');

      //il on a trouvé '\n', on le remplace par \0 (fin de chaine) pour sécuriser
      if(positionEntree != NULL)
	*positionEntree = '\0';
      
      //si on a pas trouvé, il est dans le buffer, on va donc le vider
      else
	{
	  int c = 0;
	  //EOF signifie "fin de fichier": tant qu'on a pas trouvé /n, on cherche
	  while (c != '\n' && c != EOF)
	    c = getchar();
	}
    }
  else
    {
      int c = 0;
      while (c != '\n' && c != EOF)
	c = getchar();
    }
}


//cette fonction demande à l'utilisateur sur quelle île placer un pont
void pont(int size, game jeu)
{
  /*la grille aura des coordonnées entre 0 et 99
   de plus, l'utilisateur va valider avec \n et le \0 compte
  il faut donc créer un tableau de taille 4 pour reçevoir son choix*/
  
  char pose[4], direction[NB_DIRS];
  int tailleDePose = sizeof(pose);
  
  printf("Veuillez saisir le numero de l'ile sur laquelle placer un pont\n");
  printf("les details des iles sont avant la grille:\n");
    
  //on lit la saisie de l'utilisateur proprement
  lecture_securisee(pose, tailleDePose);
  
  int base = atoi(pose);
  
  //fgets(direction, sizeof(direction), stdin);

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
  game jeu = initialise(size, nbnode, grille, t);

  //vérification des nodes
  for(int i = 0; i < nbnode; i++)
    printf("ile numero %d (degre %d) : (%d; %d) \n", i, get_required_degree(t[i]), get_x(t[i]), get_y(t[i]));

  //on affiche la grille au départ
  affichage(size, grille, jeu);

  //on demande où placer le pont
  pont(size, jeu);
  
  return EXIT_SUCCESS;
}
