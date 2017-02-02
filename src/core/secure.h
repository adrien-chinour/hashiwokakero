/*
 * fichier : game.h
 * 
 * description : ce fichier donne une liste détaillée de fonctions qui sécurisent le hashiwokakero
 */


/*
 * description : fonction qui stocke une chaîne de caractère saisie par le joueur
 * paramètre 1 : variable dans laquelle la fonction va stocker la saisie
 * paramètre 2 : longueur maximale de la chaîne que le joueur doit saisir
 * retour : true si ça s'est bien passé, false sinon
 */
bool read(char *chaine, int longueur);
