#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/secure.h"
#include "interface.h"

//TEST A AJOUTER POUR LIMITER LES ERREURS
//if( ! (atoi(value) == 0 && value != "0") ) choice = atoi(value);

int get_choice(){
  int choice = -1;
  
  while(choice < 1 || choice > 8){
    char *value = (char*) malloc(4096*sizeof(char));
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
      /*0 = Quitter*/
      case 0: delete_game(g); printf("À bientôt !\n"); break;
	
      /*1 = Aide*/
      case 1: printf("0 = Quitter\n1 = Aide\n2 = Afficher la partie\n3 = Afficher info nodes\n4 = Ajouter un pont\n5 = Supprimer un pont\n6 = Recommencer\n7 = Valider la partie\n8 = Sauvegarder la partie\n"); break;
      
      /*2 = Affiche la partie*/
      case 2: draw_hashiMap(g,m); break;
      
      /*3 = Affiche info noeuds*/
      case 3: print_game(g); break;
      
      /*4 = Ajoute un pont*/
      case 4: add_bridge(g,m); break;
      
      /*5 = Supprime un pont*/
      case 5: del_bridge(g,m); break;
      
      /*6 = Redémarre la partie*/
      case 6: reset_game(g,m); break;
      
      /*7 = test si le jeu est fini*/
      case 7: test_game_over(g); break;

      /*8 = sauvegarde le jeu*/
      case 8: save_game(g); break;
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
