#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/file.h"
#include "slist.h"

//cette fonction compte le nombre de voisins d'une île
int nb_neighbours(game g, int num)
{
  //le compteur de voisins de num va incrémenter à chaque voisin vu
  int neighbours = 0;
  
  for(int i = 0; i < game_nb_dir(g); i++)
    {
      if(get_neighbour_dir(g, num, i) != -1)
	neighbours++;
    }
  return neighbours;
}

void evidence(game g)
{
  //on parcours chaque node
  for(int i = 0; i < game_nb_nodes(g); i++)
    {
      //on regarde dans toutes les directions
      for(int j = 0; j < game_nb_dir(g); j++)
	{
	  //solution évidente 1: degré de l'île = nombre de voisins * nombre de ponts maximum
	  if(get_required_degree(game_node(g,i)) == nb_neighbours(g, i) * game_nb_max_bridges(g) && can_add_bridge_dir(g, i, j))
	    {
	      for(int k = 0; k < game_nb_max_bridges(g); k++)
	        add_bridge_dir(g, i, j);
	    }
	  //solution évidente 2: il n'y a qu'un voisin
	  else if(nb_neighbours(g, i) == 1)
	    {
	      for(int k = 0; k < game_nb_max_bridges(g); k++)
	        add_bridge_dir(g, i, j);
	    }
	  
	}
    }
}

bool solver_r(game g,int node_num,int dir){
  //si le nombre de ponts max d'une ile et atteint
  if(get_required_degree(game_node(g,node_num))==get_degree(g,node_num)){
    //si on n'a pas parcouru chaque ile, on relance la récursivité avec la suivante
    if(node_num<(game_nb_nodes(g)-1)){
      solver_r(g,node_num+1,dir);
    }
    //mais si le jeu est fini, on donne le résultat
    else{
      if(game_over(g)){
        return g;
      }
      //sinon quelque chose s'est mal passé, on supprime un pont
      else{
        if(dir>=0){
          del_bridge_dir(g,node_num,dir); //revenir en arrière
        }
      }
    } 
  }
  //si le degré de l'ile vue n'est pas atteint
  else{
    //on regarde dans toutes les directions
    for(int d=0; d<game_nb_dir(g);d++){
       if(can_add_bridge_dir(g,node_num,d)&&(!(get_required_degree(game_node(g,node_num))==get_degree(g,node_num)))&&(!(get_required_degree(game_node(g,get_neighbour_dir(g,node_num,d)))==get_degree(g,get_neighbour_dir(g,node_num,d))))){
	 //on peut poser un pont, on le pose et on relance la récursivité
        add_bridge_dir(g,node_num,d);
        solver_r(g,node_num,d);
      }
    }
  }
}


game reset_node(game g, int node_num){
  for(int i = 0; i < game_nb_dir(g); i++){
    while(get_degree_dir(g, node_num, i) > 0){
      del_bridge_dir(g,node_num, i);
    }
  }
  return g;
}

/*
  Applique la possibilité donnée par tab sur le node n dans le game g
*/
 
game apply_possibility(int * tab, game g, int node_num){
  g = reset_node(g,node_num);
  for(int i = 0; i < game_nb_dir(g); i++){
    for(int j = 0; j < tab[i]; j++){
      add_bridge_dir(g, node_num, i);
    }
  }
  return g;
}

bool check_possibility(int * tab, game g, int node_num){
  int somme = 0;
  for(int i = 0; i < game_nb_dir(g); i++){
    if(tab[i] > 0 && can_add_bridge_dir(g, node_num, i) == false){
      return false;
    }
    somme += tab[i]; 
  }
  if(somme != get_required_degree(game_node(g,node_num)))
    return false;
  return true;
}	 
	 

/*
  Fonction utilisée par create_possibilities pour générer les variantes de tab
  La fonction génère la variante du tableau dans l'ordre lexicographique
  Plus de détail : https://www.nayuki.io/page/next-lexicographical-permutation-algorithm
  Le probleme est les doublons générés lorsque deux nombres sont identiques...
*/
static int * next_possibility (int * tab, int n, int max){
  int * next_tab = malloc(sizeof(int)*n);
  int i = 0;
  while(tab[i] == max){
    if(i == n-1){
      next_tab[0] = -1;
      return next_tab;
    }
    next_tab[i] = 0;
    i++;
  }
  next_tab[i] = tab[i]+1;
  i++;
  while(i<n){
    next_tab[i] = tab[i];
    i++;
  }
  return next_tab;
}


/*
  Retourne une SList
  chaque élément de la liste contient un tableau qui caractérise une possibilté
  exemeple : [1;2;0;0] -> Un pont au nord et deux a l'ouest
 */
SList  create_possibilities(game g, int node_num){
  SList list = slist_create_empty();
  /* Générer toute les combinaisons possibles et les stocker dans la liste */
  int n = game_nb_dir(g);
  int max = game_nb_max_bridges(g);
  int * tab = malloc(sizeof(int)*n);
  for(int i = 0; i < n; i++){
    tab[i] = 0;
  }
  while(tab[0] != -1){
    if(check_possibility(tab,g,node_num)){
       list = slist_prepend(list, tab);
       printf("%d,%d,%d,%d\n",tab[0],tab[1],tab[2],tab[3]);
    }
    tab = next_possibility(tab, n, max);
  }
  printf("generation des possibilitées de %d terminée\n",node_num);
  return list;
}

/*
  Fonction principal elle effectue les appelles a toute les fonctions 
  et vérifie la validiter d'une combinaison
*/
game solve (game g){

  SList * combinaison = malloc(sizeof(SList)*game_nb_nodes(g));
  SList * start = malloc(sizeof(SList)*game_nb_nodes(g));
  /* 
     On alloue les tableaux de possibilités 
     et on applique les premières possibilités sur chaque noeud 
  */
  for(int i = 0; i < game_nb_nodes(g); i++){
    combinaison[i] = create_possibilities(g,i);
    start[i] = combinaison[i];
    g = apply_possibility(slist_data(combinaison[i]),g,i);
  }
  printf("fin initialisation\n");
  
  /* L'indice du noeud à modifier */
  int num = 0;

  /* On boucle jusqu'à la bonne combinaison :) */
  while(!game_over(g)){
    while(slist_next(combinaison[num]) == NULL){
      if(num == game_nb_nodes(g)-1){
	printf("Aucune solution trouvé :'(\n");
	return g;
      }
      combinaison[num] = start[num];
      g = apply_possibility(slist_data(combinaison[num]),g,num);
      num++;
    }
    combinaison[num] = slist_next(combinaison[num]);
    g = apply_possibility(slist_data(combinaison[num]),g,num);
    num = 0;
  }
  return g;
}

int main(int argc, char *argv[]) {
  if( argc != 3){
    printf("Erreur nombre d'argument");
    return EXIT_FAILURE;
  }
  
  game g = translate_game(argv[1]);

  if(atoi(argv[2]) == 1){
    if(solver_r(g,0,-1))
      printf("solution found !\n");
    else
      printf("solution not found !\n");
    if(game_over(g))
      printf("ok\n");
    else
      printf("not ok\n");
  }

  if(atoi(argv[2]) == 2){
    g = solve(g);
  }
  
  char * save = malloc(sizeof(char)*100);
  sprintf(save, "%s.solved",argv[1]);
  write_save(g,save);
  //printf("solution found !\n");
  free(save);
  return EXIT_SUCCESS;
}
