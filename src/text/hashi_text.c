#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/securise.c"
#include "interface.h"

//TEST A AJOUTER POUR LIMITER LES ERREURS
//if( ! (atoi(value) == 0 && value != "0") ) choice = atoi(value);

int get_choice(){
  int choice = -1;
<<<<<<< HEAD
  while(choice < 1 || choice > 8){
    char *value = (char*) malloc(4096*sizeof(char));
=======
  while(choice < 0 || choice > 8){
    char *value = (char*) malloc(sizeof(char));
>>>>>>> 77279b880df96ead55442b7b20504721c99b4619
    printf("Que souhaitez-vous faire ? (1 pour l'aide)\n");
    scanf("%s", value);
    choice = atoi(value);
    free(value);
  }
  printf("\n");
  return choice;
}

void prompt(game g, hashiMap m){ 
  int choice = -1;
  while(choice != 8){
    choice = get_choice();

    switch(choice){
      /*1 = Aide*/
<<<<<<< HEAD
      case 1: printf("1 = Aide\n2 = Afficher la partie\n3 = Afficher info nodes\n4 = Ajouter un pont\n5 = Supprimer un pont\n6 = Recommencer\n7 = Valider la partie\n8 = Quitter\n"); break;
=======
      case 1: printf("0 = Quitter\n1 = Aide\n2 = Afficher la partie\n3 = Afficher info nodes\n4 = Ajouter un pont\n5 = Supprimer un pont\n6 = Recommencer\n7 = Valider la partie\n8 = Sauvegarder la partie\n"); break;
>>>>>>> 77279b880df96ead55442b7b20504721c99b4619
      
      /*2 = Affiche la partie*/
      case 2: draw_hashiMap(g,m); break;
      
      /*3 = Affiche info noeuds*/
      case 3: print_game(g); break;
      
      /*4 = Ajoute un pont*/
      case 4: add_bridge(g,m); break;
      
      /*5 = Supprime un pont*/
      case 5: del_bridge(g,m); break;
      
      /*Redémarre la partie*/
      case 6: reset_game(g,m); break;
      
      /*test si le jeu est fini*/
      case 7: test_game_over(g); break;

<<<<<<< HEAD
      /*0 = Quitter*/
      case 8: delete_game(g); printf("À bientôt !\n"); break;
=======
       case 8: save_game(g); break;
          
>>>>>>> 77279b880df96ead55442b7b20504721c99b4619
    }
  }
}

int main() {

  starting();

  game g = game_select();

  print_game(g);

  hashiMap m = create_hashiMap(g);

  prompt(g,m);
  
  delete_hashiMap(m);

  return EXIT_SUCCESS;
}
