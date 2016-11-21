#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../include/node.h"
#include "../include/game.h"

/* Defalult configuration
   2     2     3

   5     1

   3           2
*/

#define EXAMPLE_NB_NODE 7
#define SIZE 3
#define NB_DIR 4
#define NB_MAX_BRIDGES 2

int matrix[SIZE*2-1][SIZE*2-1];
int vals[EXAMPLE_NB_NODE][3] = {{0,0,3},{0,1,5},{0,2,2},{1,1,1},{1,2,2},{2,0,2},{2,2,3}};

static game default_game (){
    node nodes[EXAMPLE_NB_NODE];
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      nodes[i] = new_node(vals[i][0],vals[i][1],vals[i][2]);
    game g = new_game(EXAMPLE_NB_NODE, nodes, NB_MAX_BRIDGES, NB_DIR);
    for (int i = 0 ; i < EXAMPLE_NB_NODE; i++)
      delete_node(nodes[i]);
    return g;
}

// End ...

// Fonction 

void starting(void){

  srand(time(NULL));

  printf("\033[1;3%dm       ___           ___           ___           ___                          \n",rand() %7+1 );
  printf("\033[1;3%dm      /\\  \\         /\\  \\         /\\__\\         /\\  \\                         \n", rand() %7+1);
  printf("\033[1;3%dm      \\:\\  \\       /::\\  \\       /:/ _/_        \\:\\  \\       ___              \n", rand() %7+1);
  printf("\033[1;3%dm       \\:\\  \\     /:/\\:\\  \\     /:/ /\\  \\        \\:\\  \\     /\\__\\             \n", rand() %7+1);
  printf("\033[1;3%dm   ___ /::\\  \\   /:/ /::\\  \\   /:/ /::\\  \\   ___ /::\\  \\   /:/__/             \n", rand() %7+1);
  printf("\033[1;3%dm  /\\  /:/\\:\\__\\ /:/_/:/\\:\\__\\ /:/_/:/\\:\\__\\ /\\  /:/\\:\\__\\ /::\\  \\             \n", rand() %7+1);
  printf("\033[1;3%dm  \\:\\/:/  \\/__/ \\:\\/:/  \\/__/ \\:\\/:/ /:/  / \\:\\/:/  \\/__/ \\/\\:\\  \\__          \n", rand() %7+1);
  printf("\033[1;3%dm   \\::/__/       \\::/__/       \\::/ /:/  /   \\::/__/       ~~\\:\\/\\__\\         \n", rand() %7+1);
  printf("\033[1;3%dm    \\:\\  \\        \\:\\  \\        \\/_/:/  /     \\:\\  \\          \\::/  /         \n", rand() %7+1);
  printf("\033[1;3%dm     \\:\\__\\        \\:\\__\\         /:/  /       \\:\\__\\         /:/  /          \n", rand() %7+1);
  printf("\033[1;3%dm      \\/__/         \\/__/         \\/__/         \\/__/         \\/__/           \n", rand() %7+1);
  printf("\033[1;3%dm                               ___           ___                    \n", rand() %7+1);
  printf("\033[1;3%dm                              /\\__\\         /|  |                   \n", rand() %7+1);
  printf("\033[1;3%dm                 ___         /:/ _/_       |:|  |           ___     \n", rand() %7+1);
  printf("\033[1;3%dm                /\\__\\       /:/ /\\__\\      |:|  |          /\\__\\    \n", rand() %7+1);
  printf("\033[1;3%dm               /:/  /      /:/ /:/ _/_   __|:|__|         /:/  /    \n", rand() %7+1);
  printf("\033[1;3%dm              /:/__/      /:/_/:/ /\\__\\ /::::\\__\\_____   /:/__/     \n", rand() %7+1);
  printf("\033[1;3%dm             /::\\  \\      \\:\\/:/ /:/  / ~~~~\\::::/___/  /::\\  \\     \n", rand() %7+1);
  printf("\033[1;3%dm            /:/\\:\\  \\      \\::/_/:/  /      |:|~~|     /:/\\:\\  \\    \n", rand() %7+1);
  printf("\033[1;3%dm            \\/__\\:\\  \\      \\:\\/:/  /       |:|  |     \\/__\\:\\  \\   \n", rand() %7+1);
  printf("\033[1;3%dm                 \\:\\__\\      \\::/  /        |:|__|          \\:\\__\\  \n", rand() %7+1);
  printf("\033[1;3%dm                  \\/__/       \\/__/         |/__/            \\/__/      \n", rand() %7+1);

  printf("\033[0;m \n"); 
}

void create_matrix(cgame g){
  //Initialisation de matrix 
  for(int i = 0; i < SIZE*2-1; i++){
    for(int j = 0 ; j < SIZE*2-1 ; j++){
      matrix[i][j] = 0;
    }
  }

  //ajout des nodes dans matrix
  for(int i = 0; i < game_nb_nodes(g); i++){
    int x = get_x(game_node(g, i));
    int y = get_y(game_node(g, i));
    matrix[y*2][x*2] = 1;
  }
}

void print_matrix(cgame g){
  
  // affichage coordonnées abscisses
  for(int i = 0; i < SIZE*2-1; i++){
    if(i%2 == 0)
      printf("%d  ", i/2);
    else
      printf("   ");
  }
  printf("\n");
  
  // affichage ligne abscisses
  for(int i = 0; i < SIZE*2-1; i++){
    printf("---");
  }
  printf("\n");
  
  //affichage de matrix
  for(int i = SIZE*2-2; i >= 0; i--){
    for(int j = 0 ; j < SIZE*2-1 ; j++){
      switch(matrix[i][j]){

        case 0: // vide
          printf(".  ");
          break;

        case 1: // node
          if(get_required_degree(game_node(g, game_get_node_number(g, j/2, i/2))) == get_degree(g, game_get_node_number(g, j/2, i/2)))
            printf("\033[32m%d  \033[00m", get_required_degree(game_node(g ,game_get_node_number(g,j/2,i/2))));
          else
            printf("\033[31m%d  \033[00m", get_required_degree(game_node(g ,game_get_node_number(g,j/2,i/2))));

          break;

        case 2: // bridge
          printf("+  ");
          break;

        case 3: // two bridges 
          printf("#  ");
          break;

        default: // error
          printf("ERR");
          break;
      }
    }

    //affichage ligne et coordonnées ordonnées
    if (i%2==0)
      printf ("| %d\n\n", i/2);
    else
       printf("|\n\n");
  }  
}



void print_game(cgame g){
  for(int i = 0; i < game_nb_nodes(g); i++){
    if (get_required_degree(game_node(g, i)) == get_degree(g, i))
      printf("\033[32mnode n°%d : x = %d , y = %d ; degré actuel : %d ; degré necessaire : %d\n", 
                                                                i, 
                                                                get_x(game_node(g, i)), 
                                                                get_y(game_node(g, i)), 
                                                                get_degree(g, i),
                                                                get_required_degree(game_node(g,i)));
    else
      printf("\033[31mnode n°%d : x = %d , y = %d ; degré actuel : %d ; degré necessaire : %d\n", 
                                                                i, 
                                                                get_x(game_node(g, i)), 
                                                                get_y(game_node(g, i)), 
                                                                get_degree(g, i),
                                                                get_required_degree(game_node(g,i)));
  }
  printf("\033[00m\n");
}


void add_bridge(game g){
  int node_num = -1;
  while(node_num < 0 || node_num >= game_nb_nodes(g)){
    char *value = (char*) malloc(sizeof(char));
    printf("Sur quelle noeud voulez-vous ajouter un pont ?\n");
    scanf("%s", value);
    node_num = atoi(value);
    free(value);
  }

  int direction = 0;
  while(direction < 1 || direction > 4){
    char *value = (char*) malloc(sizeof(char));
    printf("Dans quelle direction ?\n  1 = NORD / 2 = SUD / 3 = EST / 4 = OUEST\n");
    scanf("%s", value);
    direction = atoi(value);
    free(value);
  }

  if(direction == 1){
    if(can_add_bridge_dir(g, node_num, NORTH)){
      add_bridge_dir(g, node_num, NORTH);
      int i = get_y(game_node(g, node_num))*2+1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NORTH)))*2){
        matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, NORTH)+1;
        i++;
      }
    }
    else
      printf("Impossible d'ajouter un pont\n");
  }
  else if(direction == 2){
    if(can_add_bridge_dir(g, node_num, SOUTH)){
      add_bridge_dir(g, node_num, SOUTH);
      int i = get_y(game_node(g, node_num))*2-1;
      while(i > get_y(game_node(g, get_neighbour_dir(g, node_num, SOUTH)))*2){
        matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, SOUTH)+1;
        i--;
      }
    }
    else
      printf("Impossible d'ajouter un pont\n");
  }
  else if(direction == 3){
    if(can_add_bridge_dir(g, node_num, EAST)){
      add_bridge_dir(g, node_num, EAST);
      int i = get_x(game_node(g, node_num))*2+1;
      while(i < get_x(game_node(g, get_neighbour_dir(g, node_num, EAST)))*2){
        matrix[get_y(game_node(g, node_num))*2][i] = get_degree_dir(g, node_num, EAST)+1;
        i++;
      }
    }
    else
      printf("Impossible d'ajouter un pont\n");
  }
  else if(direction == 4){
    if(can_add_bridge_dir(g, node_num, WEST)){
      add_bridge_dir(g, node_num, WEST);
      int i = get_x(game_node(g, node_num))*2-1;
      while(i > get_x(game_node(g, get_neighbour_dir(g, node_num, WEST)))*2){
        matrix[get_y(game_node(g, node_num))*2][i] = get_degree_dir(g, node_num, WEST)+1;
        i--;
      }
    }
    else
      printf("Impossible d'ajouter un pont\n");
  }

}

void del_bridge(game g){
  int node_num = -1;
  while(node_num < 0 || node_num >= game_nb_nodes(g)){
    char *value = (char*) malloc(sizeof(char));
    printf("Sur quelle noeud voulez-vous supprimer un pont ?\n");
    scanf("%s", value);
    node_num = atoi(value);
    free(value);
  }

  int direction = 0;
  while(direction < 1 || direction > 4){
    char *value = (char*) malloc(sizeof(char));
    printf("Dans quelle direction ? ( 1 = NORD / 2 = SUD / 3 = EST / 4 = OUEST ) \n");
    scanf("%s", value);
    direction = atoi(value);
    free(value);
  }

  if(direction == 1){
    del_bridge_dir(g, node_num, NORTH);
    int i = get_y(game_node(g, node_num))*2+1;
    while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NORTH)))*2){
      if(get_degree_dir(g, node_num, NORTH) == 0)
        matrix[i][get_x(game_node(g, node_num))*2] = 0;
      else 
        matrix[i][get_x(game_node(g, node_num))*2] = 2;
      i++;
    }
  }
  else if(direction == 2){
    del_bridge_dir(g, node_num, SOUTH);
    int i = get_y(game_node(g, node_num))*2-1;
    while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, SOUTH)))*2){
      if(get_degree_dir(g, node_num, SOUTH) == 0)
        matrix[i][get_x(game_node(g, node_num))*2] = 0;
      else 
        matrix[i][get_x(game_node(g, node_num))*2] = 2;
      i++;
    }
  }
  else if(direction == 3){
    del_bridge_dir(g, node_num, EAST);
    int i = get_x(game_node(g, node_num))*2+1;
    while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, EAST)))*2){
      if(get_degree_dir(g, node_num, EAST) == 0)
        matrix[get_y(game_node(g, node_num))*2][i] = 0;
      else 
        matrix[get_y(game_node(g, node_num))*2][i] = 2;
      i++;
    }
  }
  else if(direction == 4){
    del_bridge_dir(g, node_num, WEST);
    int i = get_x(game_node(g, node_num))*2-1;
    while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, WEST)))*2){
      if(get_degree_dir(g, node_num, WEST) == 0)
        matrix[get_y(game_node(g, node_num))*2][i] = 0;
      else 
        matrix[get_y(game_node(g, node_num))*2][i] = 2;
      i++;
    }
  }
}

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

void reset_game(game g){
  //supression des ponts
  for(int i = 0; i < game_nb_nodes(g); i++){
    for(int j = 0; j < 4; j++){
      if(get_degree_dir(g, i, j) == 2)
        del_bridge_dir(g,i,j);
      if(get_degree_dir(g, i, j) == 1)
        del_bridge_dir(g,i,j);
    }
  }

  // reinitialisation de matrix
  for(int i = 0; i < SIZE*2-1; i++){
    for(int j = 0 ; j < SIZE*2-1 ; j++){
      if(matrix[i][j] != 1)
        matrix[i][j] = 0;
    }
  }
}

void test_game_over(game g){
  if(game_over(g)){
    printf("BRAVO, VOUS AVEZ GAGNÉ ! \n");
    print_matrix(g);
    print_game(g);
    exit(EXIT_SUCCESS);
  }
  else
    printf("Vous n'avez pas encore fini...\n");
}

void prompt(game g){
  int choice = -1;
  while(choice != 0){
    choice = get_choice();

    switch(choice){
      case 0:
        delete_game(g);
        printf("À bientôt !\n");
        break;
      case 1:
        printf("0 = Quitter\n1 = Aide\n2 = Afficher la partie\n3 = Afficher info nodes\n4 = Ajouter un pont\n5 = Supprimer un pont\n6 = Recommencer\n7 = Valider la partie\n");
        break;
      case 2:
        print_matrix(g);
        break;
      case 3:
        print_game(g);
        break;
      case 4:
        add_bridge(g);
        break;
      case 5:
        del_bridge(g);
        break;
      case 6:
        reset_game(g);
        break;
      case 7:
        test_game_over(g);
        break;
    }

  }
}

int main() {

  starting(); 

  game g = default_game();

  create_matrix(g);

  print_matrix(g);

  prompt(g);
  
  return EXIT_SUCCESS;
}
