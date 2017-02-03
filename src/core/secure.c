#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "secure.h"

//fonction de lecture sécurisée d'une chaîne de caractère écrite par l'utilisateur
bool read(char *chaine, int longueur)
{
  //on va chercher où se trouve '\n'
  char *enter = NULL;

  //on regarde si aucune erreur de saisie n'est faite
  if(fgets(chaine, longueur, stdin) != NULL)
    {
      //on cherche '/n'
      enter = strchr(chaine, '\n');

      //si on l'a trouvé, on le remplace par '\0'
      if(enter != NULL)
	*enter = '\0';

      //sinon il faut vider le buffer
      else
        {
	  int c = 0;

	  //tant que '\n' est pas trouvé et que le buffer n'est pas vide, on le vide
	  while (c != '\n' && c != EOF)
	    c = getchar();
        }
      //la saisie s'est bien passée, on le signale
      return true;
    }
  //si une erreur de saisie est détectée...
  else
    {
      int c = 0;

      //... on vide le buffer
      while (c != '\n' && c != EOF)
	c = getchar();

      //la saisie s'est mal passée, on le signale
      return false;
    }
}

/*
void until(int min, int max)
{
  while(min <= max)
    {
      
      min++;
    }
}
*/
