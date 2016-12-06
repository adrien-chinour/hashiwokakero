#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "../include/node.h"
#include "../include/game.h"
#include "../include/generate.h"

typedef struct s_map {
  int size;
  int** matrix;
} *hashiMap;


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

void draw_hashiMap(cgame g, hashiMap m){
  
  // affichage coordonnées abscisses
  for(int i = 0; i < m->size; i++){
    if(i%2 == 0)
      printf("%d  ", i/2);
    else
      printf("   ");
  }
  printf("\n");
  
  // affichage ligne abscisses
  for(int i = 0; i < m->size; i++){
    printf("---");
  }
  printf("\n");
  
  //affichage de matrix
  for(int i = m->size-1; i >= 0; i--){
    for(int j = 0 ; j < m->size ; j++){
      switch(m->matrix[i][j]){

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

void add_bridge(game g, hashiMap m){
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


  if(can_add_bridge_dir(g,node_num, direction-1)){
    add_bridge_dir(g, node_num, direction-1);
    int i = 0;
    switch(direction){
      case 1: //NORTH
        i = get_y(game_node(g, node_num))*2+1;
        while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NORTH)))*2){
          m->matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, NORTH)+1;
          i++;
        }
        break;
      case 2: //SOUTH
        i = get_y(game_node(g, node_num))*2-1;
        while(i > get_y(game_node(g, get_neighbour_dir(g, node_num, SOUTH)))*2){
          m->matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, SOUTH)+1;
          i--;
        }
        break;
      case 3: //
        i = get_x(game_node(g, node_num))*2+1;
        while(i < get_x(game_node(g, get_neighbour_dir(g, node_num, EAST)))*2){
          m->matrix[get_y(game_node(g, node_num))*2][i] = get_degree_dir(g, node_num, EAST)+1;
          i++;
        }
        break;
      case 4: 
        i = get_x(game_node(g, node_num))*2-1;
        while(i > get_x(game_node(g, get_neighbour_dir(g, node_num, WEST)))*2){
          m->matrix[get_y(game_node(g, node_num))*2][i] = get_degree_dir(g, node_num, WEST)+1;
          i--;
        }
        break;
      case 5: 
        i = get_y(game_node(g, node_num))*2+1;
        while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NW)))*2){
          m->matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, NW)+1;
          i++;
        }
        break;
      case 6: 
        i = get_y(game_node(g, node_num))*2+1;
        while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, SW)))*2){
          m->matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, SW)+1;
          i++;
        }
        break;
      case 7: 
        i = get_y(game_node(g, node_num))*2+1;
        while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NORTH)))*2){
          m->matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, NORTH)+1;
          i++;
        }
        break;
      case 8: 
        i = get_y(game_node(g, node_num))*2+1;
        while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NORTH)))*2){
          m->matrix[i][get_x(game_node(g, node_num))*2] = get_degree_dir(g, node_num, NORTH)+1;
          i++;
        }
        break;
    }
  }
  else
      printf("Impossible d'ajouter un pont\n");

}

void del_bridge(game g, hashiMap m){
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

  int i = 0;
  switch(direction){
    case 1: //NORTH
      del_bridge_dir(g, node_num, NORTH);
      i = get_y(game_node(g, node_num))*2+1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NORTH)))*2){
        if(get_degree_dir(g, node_num, NORTH) == 0) 
          m->matrix[i][get_x(game_node(g, node_num))*2] = 0;
        else m->matrix[i][get_x(game_node(g, node_num))*2] = 2;
        i++;
      }
      break;
    case 2: //SOUTH
      del_bridge_dir(g, node_num, SOUTH);
      i = get_y(game_node(g, node_num))*2-1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, SOUTH)))*2){
        if(get_degree_dir(g, node_num, SOUTH) == 0)
          m->matrix[i][get_x(game_node(g, node_num))*2] = 0;
        else m->matrix[i][get_x(game_node(g, node_num))*2] = 2;
        i++;
      }
      break;
    case 3: //EAST
      del_bridge_dir(g, node_num, EAST);
      i = get_x(game_node(g, node_num))*2+1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, EAST)))*2){
        if(get_degree_dir(g, node_num, EAST) == 0)
          m->matrix[get_y(game_node(g, node_num))*2][i] = 0;
        else m->matrix[get_y(game_node(g, node_num))*2][i] = 2;
        i++;
      }
      break;
    case 4: //WEST
      del_bridge_dir(g, node_num, WEST);
      i = get_x(game_node(g, node_num))*2-1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, WEST)))*2){
        if(get_degree_dir(g, node_num, WEST) == 0)
          m->matrix[get_y(game_node(g, node_num))*2][i] = 0;
        else m->matrix[get_y(game_node(g, node_num))*2][i] = 2;
        i++;
      }
      break;
    case 5:
      del_bridge_dir(g, node_num, NW);
      i = get_x(game_node(g, node_num))*2-1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NW)))*2){
        if(get_degree_dir(g, node_num, NW) == 0)
          m->matrix[get_y(game_node(g, node_num))*2][i] = 0;
        else m->matrix[get_y(game_node(g, node_num))*2][i] = 2;
        i++;
      }
      break;
    case 6:
      del_bridge_dir(g, node_num, SW);
      i = get_x(game_node(g, node_num))*2-1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, SW)))*2){
        if(get_degree_dir(g, node_num, SW) == 0)
          m->matrix[get_y(game_node(g, node_num))*2][i] = 0;
        else m->matrix[get_y(game_node(g, node_num))*2][i] = 2;
        i++;
      }
      break;
    case 7:
      del_bridge_dir(g, node_num, SE);
      i = get_x(game_node(g, node_num))*2-1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, SE)))*2){
        if(get_degree_dir(g, node_num, SE) == 0)
          m->matrix[get_y(game_node(g, node_num))*2][i] = 0;
        else m->matrix[get_y(game_node(g, node_num))*2][i] = 2;
        i++;
      }
      break;
    case 8:
      del_bridge_dir(g, node_num, NE);
      i = get_x(game_node(g, node_num))*2-1;
      while(i < get_y(game_node(g, get_neighbour_dir(g, node_num, NE)))*2){
        if(get_degree_dir(g, node_num, NE) == 0)
          m->matrix[get_y(game_node(g, node_num))*2][i] = 0;
        else m->matrix[get_y(game_node(g, node_num))*2][i] = 2;
        i++;
      }
      break;
  }
}

hashiMap create_hashiMap(cgame g){
  
  //récupération de size en fonction des coordonnées des nodes
  int size = 0;
  for(int i = 0; i < game_nb_nodes(g); i++){
    if((get_x(game_node(g, i))+1)*2 > size) size = (get_x(game_node(g, i))+1)*2;
    if((get_y(game_node(g, i))+1)*2 > size) size = (get_y(game_node(g, i))+1)*2;
  }

  //allocation de la map
  hashiMap m = (hashiMap) malloc(sizeof(struct s_map));
  assert(m != NULL);
  
  //ajout de size
  m->size = size;

  //allocation de matrix
  int ** matrix = (int**) malloc(sizeof(int*)*size);
  assert(matrix != NULL);
  for(int i = 0; i < size; i++){
    matrix[i] = (int*) malloc(sizeof(int)*size);
    assert(matrix[i] != NULL);
    for(int j = 0; j < size; j++){
      matrix[i][j] = 0;
    }
  }

  //ajout des nodes dans matrix
  for(int i = 0; i < game_nb_nodes(g); i++){
    int x = get_x(game_node(g, i));
    int y = get_y(game_node(g, i));
    matrix[y*2][x*2] = 1;
  }

  //ajout du pointeur sur matrix dans la struct map
  m->matrix = matrix;

  return m;
}

void delete_hashiMap(hashiMap m){
  for(int i = 0; i < m->size; i++){
    free(m->matrix[i]);
  }
  free(m->matrix);
  free(m);
}

void test_game_over(game g){
  if(game_over(g)){
    printf("BRAVO, VOUS AVEZ GAGNÉ ! \n");
    exit(EXIT_SUCCESS);
  }
  else
    printf("Vous n'avez pas encore fini...\n");
}

void reset_game(game g, hashiMap m){
  //supression des ponts
  for(int i = 0; i < game_nb_nodes(g); i++){
    for(int j = 0; j < 4; j++){
      if(get_degree_dir(g, i, j) == 2)
        del_bridge_dir(g,i,j);
      if(get_degree_dir(g, i, j) == 1)
        del_bridge_dir(g,i,j);
    }
  }

  // reinitialisation de map
  for(int i = 0; i < m->size; i++){
    for(int j = 0 ; j < m->size ; j++){
      if(m->matrix[i][j] != 1)
        m->matrix[i][j] = 0;
    }
  }
}

game game_select(){
  int choice = 0;
  while(choice < 1 || choice > 2){
    char *value = (char*) malloc(sizeof(char));
    printf("Selectionnez votre partie :\n");
    printf("-> 1 : FACILE - 4DIRS \n-> 2 : FACILE - 8DIRS \n-> 3 : MOYEN - 4DIRS \n-> 4 : MOYEN - 8DIRS \n-> 5 : DIFFICILE - 4DIRS \n-> 6 : DIFFICILE - 8DIRS\n");
    scanf("%s", value);
    choice = atoi(value);
    free(value);
  }

  switch(choice){
    case 1: return generate_game(1,2,4,7);
    case 2: printf("pas encore dispo.\n"); return generate_game(1,2,4,7);
    case 3: printf("pas encore dispo.\n"); return generate_game(1,2,4,7);
    case 4: printf("pas encore dispo.\n"); return generate_game(1,2,4,7);
    case 5: printf("pas encore dispo.\n"); return generate_game(1,2,4,7);
    case 6: printf("pas encore dispo.\n"); return generate_game(1,2,4,7);
    default: printf("Erreur dans game_select\n"); exit(EXIT_FAILURE); break;
  }
}