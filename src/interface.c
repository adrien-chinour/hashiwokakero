#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../include/node.h"
#include "../include/game.h"
#include "../include/generate.h"

/* GLOBAL DEF */

#define SIZE 5
int matrix[SIZE*2-1][SIZE*2-1];

/*FIN GLOBAL DEF */

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
  else if(direction == 5){
    if(can_add_bridge_dir(g, node_num, NW)){
      add_bridge_dir(g, node_num, NW);
      int i = get_y(game_node(g, node_num))*2+1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NW)))*2){
        matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, NW)+1;
        i++;
      }
    }
    else
      printf("Impossible d'ajouter un pont\n");
  }
  else  if(direction == 6){
    if(can_add_bridge_dir(g, node_num, SW)){
      add_bridge_dir(g, node_num, SW);
      int i = get_y(game_node(g, node_num))*2+1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, SW)))*2){
        matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, SW)+1;
        i++;
      }
    }
    else
      printf("Impossible d'ajouter un pont\n");
  }
  else  if(direction == 7){
    if(can_add_bridge_dir(g, node_num, SE)){
      add_bridge_dir(g, node_num, SE);
      int i = get_y(game_node(g, node_num))*2+1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, SE)))*2){
        matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, SE)+1;
        i++;
      }
    }
    else
      printf("Impossible d'ajouter un pont\n");
  }
  else  if(direction == 8){
    if(can_add_bridge_dir(g, node_num, NE)){
      add_bridge_dir(g, node_num, NE);
      int i = get_y(game_node(g, node_num))*2+1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NE)))*2){
        matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, NE)+1;
        i++;
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
  else if(direction == 5){
    del_bridge_dir(g, node_num, NW);
    int i = get_x(game_node(g, node_num))*2-1;
    while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NW)))*2){
      if(get_degree_dir(g, node_num, NW) == 0)
        matrix[get_y(game_node(g, node_num))*2][i] = 0;
      else 
        matrix[get_y(game_node(g, node_num))*2][i] = 2;
      i++;
    }
  }
  else if(direction == 6){
    del_bridge_dir(g, node_num, SW);
    int i = get_x(game_node(g, node_num))*2-1;
    while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, SW)))*2){
      if(get_degree_dir(g, node_num, SW) == 0)
        matrix[get_y(game_node(g, node_num))*2][i] = 0;
      else 
        matrix[get_y(game_node(g, node_num))*2][i] = 2;
      i++;
    }
  }
  else if(direction == 7){
    del_bridge_dir(g, node_num, SE);
    int i = get_x(game_node(g, node_num))*2-1;
    while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, SE)))*2){
      if(get_degree_dir(g, node_num, SE) == 0)
        matrix[get_y(game_node(g, node_num))*2][i] = 0;
      else 
        matrix[get_y(game_node(g, node_num))*2][i] = 2;
      i++;
    }
  }
  else if(direction == 8){
    del_bridge_dir(g, node_num, NE);
    int i = get_x(game_node(g, node_num))*2-1;
    while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NE)))*2){
      if(get_degree_dir(g, node_num, NE) == 0)
        matrix[get_y(game_node(g, node_num))*2][i] = 0;
      else 
        matrix[get_y(game_node(g, node_num))*2][i] = 2;
      i++;
    }
  }
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

game game_select(){
  int choice = 0;
  while(choice < 1 || choice > 2){
    char *value = (char*) malloc(sizeof(char));
    printf("Selectionnez votre partie :\n");
    printf("1 : FACILE - 4DIRS / 2 : FACILE - 8DIRS / 3 : MOYEN - 4DIRS / 4 : MOYEN - 8DIRS / 5 : DIFFICILE - 4DIRS / 6 : DIFFICILE - 8DIRS\n");
    scanf("%s", value);
    choice = atoi(value);
    free(value);
  }

  switch(choice){
    case 1: return generate_game(1,2,4);
    default: return NULL;
  }
}