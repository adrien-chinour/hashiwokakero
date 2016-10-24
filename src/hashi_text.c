#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../include/node.h"
#include "../include/game.h"

//cette fonction vérifie si l'utilisateur lance le programme correctement
void securise(int *argc, char *argv[], int *size, int *nbnode)
{
  /*lors du lancement, l'utilisateur doit choisir:
    > la taille de la grille de jeu (2 à 30)
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
    printf("Erreur : veuillez entrer la taille entre 2 et 30 et les nodes entre 1 et 900 !\n");
    printf("Synthaxe : ./hashi_text TAILLE NBNODE\n");
    exit(EXIT_FAILURE);
  }

  //si la taille du jeu est hors normes (taille < 2 ou taille > 30), on indique le problème
  if(*size < 2 || *size > 30)
    {
      printf("Erreur : la grille de jeu choisie ne peut pas etre construite (choisissez de 2 à 30) !\n");
      printf("Synthaxe : ./hashi_text TAILLE NBNODE\n");
      exit(EXIT_FAILURE);
    }
  *size = 2*(*size)-1;
}


//cette fonction affiche l'état actuel du jeu
void affichage(int size, int grille[size][size], game jeu)
{
  //affiche l'axe des abscisses
  for(int i = 0; i < size; i++)
    {
      if(i%2 == 0)
	{
	  if(i < 20)
	    printf(" %d  ", i/2);
	  else
	    printf(" %d ", i/2);
	}
      else
	printf("    ");
    }
  printf("\n");
  for(int i = 0; i < size; i++)
    printf("----");
  printf("\n");

  //affiche toute la grille
  for(int y = size-1; y >= 0; y--)
    {
      for(int x = 0; x < size; x++)
	{
	  switch (grille[x][y])
	    {
	    case -2: //affiche un pont simple
	      printf(" +  ");
	      break;
	    case -3: //affiche un pont double
	      printf(" #  ");
	      break;
	    case -1: //affiche une case vide
	      printf(" .  "); 
	      break;
	    default: //affiche le degré d'une île
	      printf(" %d  ", grille[x][y]);
	      break;
	    }
	}
      //affiche l'axe des ordonnées
      if(y%2 == 0)
	printf("| %d\n\n", y/2);
      else
	printf("|\n\n");
    }
  printf("-- fin de la zone de jeu --\n");
}


//si une coordonnée est impaire, on la rend paire
int rend_pair(int nombre)
{
  if(nombre % 2 != 0)
    return nombre-1;
  return nombre;
}


//cette fonction détruit toute la grille et en recrée une à initialiser
void reset(game jeu, int nbnode, node t[])
{
  //on détruit tous les ponts et les nodes
  for(int i = 0; i < nbnode; i++)
    {
      for(int j = 0; j < NB_DIRS; j++)
	{
	  if(get_degree_dir(jeu, i, j) == 2)
	    del_bridge_dir(jeu, i, j);
	  if(get_degree_dir(jeu, i, j) == 1)
	    del_bridge_dir (jeu, i, j);
	}
      delete_node(t[i]);
    }
  delete_game(jeu);
}


//cette fonction enlève tous les ponts de la grille (faut pas donner la solution dès le début :p)
void del_all_bridges(int nbnode, game jeu)
{
  for(int i = 0; i < nbnode; i++)
    {
      for(int j = 0; j < NB_DIRS; j++)
	{
	  if(get_degree_dir(jeu, i, j) == 2)
	    del_bridge_dir(jeu, i, j);
	  if(get_degree_dir(jeu, i, j) == 1)
	    del_bridge_dir (jeu, i, j);
	}
    }
}


//cette fonction va créer des tableaux pour le jeu
game initialise(int size, int nbnode, int grille[size][size], node t[])
{
  //on initialise la grille à -1 (case vide)
  for(int i = 0; i < size; i++)
    {
      for(int j = 0; j < size; j++)
	grille[i][j] = -1;
    }
  
  //on crée des nodes aléatoires
  srand(time(NULL));

  //la node de base (qui permettra de placer les autres comme il faut)
  int x = rend_pair(rand()%size), y = rend_pair(rand()%size);
  t[0] = new_node(x, y, 0);
  grille[x][y] = 0;

  //on prend une coordonnée de la node précédente et l'autre est aléatoire, puis on place une autre node
  for(int i = 1; i < nbnode; i++)
    {
      int randX = x, randY = y;

      while(grille[randX][randY] == 0)
	{
	  int pileFace = rand()%2;
	  
	  if(pileFace == 0)
	    randX = rend_pair(rand()%size);
	  else
	    randY = rend_pair(rand()%size);
	}
      x = randX;
      y = randY;
      t[i] = new_node(x, y, 0);
      grille[x][y] = 0;
    }

  //création du jeu à partir des nodes précédement créées
  game jeu = new_game(nbnode, t);
  
  //affichage(size, grille, jeu);
  
  //on cherche à placer aléatoirement des ponts sur chaque node pour leur définir un degré
  for(int i = 0; i < nbnode; i++)
    {
      for(int j = 0; j < NB_DIRS; j++)
	{
	  int voisin = get_neighbour_dir(jeu, i, j), pileFace = rand()%2;
	  
	  //si un voisin est détecté et qu'il n'y a pas d'obstacle on pose un pont ou pas
	  if(pileFace == 0 && voisin != -1 && can_add_bridge_dir(jeu, i, j))
	    add_bridge_dir(jeu, i, j);
	  else if(voisin != -1 && can_add_bridge_dir(jeu, i, j) && get_degree_dir(jeu, i, j) == 0)
	    {
	      add_bridge_dir(jeu, i, j);
	      if(pileFace == 0)
		add_bridge_dir(jeu, i, j);
	    }
	}
    }

  //on défini le degré de chaque node selon le nombre de ponts dessus
  for(int i = 0; i < nbnode; i++)
    {
      int x = get_x(t[i]), y = get_y(t[i]);
      grille[x][y] = get_degree(jeu, i);
    }
  //affichage(size, grille, jeu);
  return jeu;
}


//cette fonction analyse une commande du joueur pour faire une action
void commande(char faire[], int n)
{
  //char posePont = "place", retirePont = "prend", afficheNode = "stats", reset = "reset", commande = "commande";

  //switch
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
  commande(chaine, longueur);
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

  printf("Veuillez entrer une direction (NORD; SUD; EST; OUEST) pour y placer un pont\n");

  lecture_securisee(pose, tailleDePose);
  int dirige = atoi(pose);

  can_add_bridge_dir(jeu, base, dirige);
}


//cette fonction vérifie la connexité du jeu
bool connexe(int nbnode, game jeu)
{
  for(int i = 1; i < nbnode; i++)
    {
      for(int j = 0; j < NB_DIRS; j++)
	{
	  if(get_neighbour_dir(jeu, i, j) == i-1 && get_degree_dir(jeu, i, j) == 0)
	    return false;
	}
    }
  return true;
}


//cette fonction affiche un tableau avec des informations sur chaque noeud
void print_nodes_detail(int nbnode, node t[], int size, cgame jeu){
	printf("  X:  |  Y:  |  °:  | OK? |  N  \n");
	
	//boucles pour tri noeud par x puis par y
	for(int x = 0; x < size; x++){
		for(int y = 0; y < size; y++){
			for(int i = 0; i < nbnode; i++){
				if(get_x(t[i]) == x && get_y(t[i]) == y){
					char a;

					//test si nombre de pont egal au degre
					if (get_required_degree(t[i]) == 0) //test a voir 
						a = 'V';
					else
						a = 'X';

					//test de la longeur de la chaine pour affichage identique
					if(get_x(t[i]) > 19 && (get_y(t[i]) < 19))
						printf("  %d  |  %d   |  %d   |  %c  |  %d\n", 
																get_x(t[i])/2, 
																get_y(t[i])/2, 
																get_required_degree(t[i]), 
																a,
																i);

					else if(get_y(t[i]) > 19 && (get_x(t[i]) < 19))
						printf("  %d   |  %d  |  %d   |  %c  |  %d\n", 
																get_x(t[i])/2, 
																get_y(t[i])/2, 
																get_required_degree(t[i]), 
																a,
																i);

					else if(get_x(t[i]) > 19 && (get_y(t[i]) > 19))
						printf("  %d  |  %d  |  %d   |  %c  |  %d\n", 
																get_x(t[i])/2, 
																get_y(t[i])/2, 
																get_required_degree(t[i]), 
																a,
																i);
					else
						printf("  %d   |  %d   |  %d   |  %c  |  %d\n", 
																get_x(t[i])/2, 
																get_y(t[i])/2, 
																get_required_degree(t[i]), 
																a,
																i);

				}
			}
		}
	}
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

  //on réinitialise la grille au moindre problème d'initialisation
  while(game_over(jeu) == false && connexe(nbnode, jeu) == false)
    {
      reset(jeu, nbnode, t);
      jeu = initialise(size, nbnode, grille, t);
    }
  del_all_bridges(nbnode, jeu);
  
  //vérification des nodes
  print_nodes_detail(nbnode, t, size, jeu);

  //on affiche la grille au départ
  affichage(size, grille, jeu);

  //on demande où placer le pont
  pont(size, jeu);
  
  return EXIT_SUCCESS;
}
