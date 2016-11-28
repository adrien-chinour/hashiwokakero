#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../include/node.h"
#include "../include/game.h"
#include "../include/interface.h"

// /* DECLARATION GAME */

// #define EXAMPLE_NB_NODE 7
// #define NB_DIR 4
// #define NB_MAX_BRIDGES 2


// int vals[EXAMPLE_NB_NODE][3] = {{0,0,3},{0,2,5},{0,4,2},{2,2,1},{2,4,2},{4,0,2},{4,4,3}};

// static game default_game (){
//     node nodes[EXAMPLE_NB_NODE];
//     for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
//       nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
//     game g = new_game(EXAMPLE_NB_NODE, nodes, NB_MAX_BRIDGES, NB_DIR);
//     for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
//       delete_node(nodes[i]);
//     return g;
// }
// /* FIN DECLARATION GAME */

int get_choice(){
  int choice = -1;
  while(choice < 0 || choice > 7){
    char *value = (char*) malloc(sizeof(char));
    printf("Que souhaitez-vous faire ? (1 pour l'aide)\n");
    scanf("%s", value);
    choice = atoi(value);
    free(value);
  }
  printf("\n");
  return choice;
}

void prompt(game g){
  int choice = -1;
  while(choice != 0){
    choice = get_choice();

    switch(choice){
      /*0 = Quitter*/
      case 0: delete_game(g); printf("À bientôt !\n"); break;

      /*1 = Aide*/
      case 1: printf("0 = Quitter\n1 = Aide\n2 = Afficher la partie\n3 = Afficher info nodes\n4 = Ajouter un pont\n5 = Supprimer un pont\n6 = Recommencer\n7 = Valider la partie\n"); break;
      
      /*2 = Affiche la partie*/
      case 2: print_matrix(g); break;
      
      /*3 = Affiche info noeuds*/
      case 3: print_game(g); break;
      
      /*4 = Ajoute un pont*/
      case 4: add_bridge(g); break;
      
      /*5 = Supprime un pont*/
      case 5: del_bridge(g); break;
      
      /*Redémarre la partie*/
      case 6: reset_game(g); break;
      
      /*test si le jeu est fini*/
      case 7: test_game_over(g); break;
    }
  }
}

int main() {

  starting(); 

  game g = game_select();

  create_matrix(g);

  prompt(g);
  
  return EXIT_SUCCESS;
}
