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
  
  for(int i = 0; i < game_nb_dirs(g); i++)
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
      for(int j = 0; j < game_nb_dirs(g); j++)
	{
	  //solution évidente 1: degré de l'île = nombre de voisins * nombre de ponts maximum
	  if(get_required_degree(g, i) == nb_neighbours(g, i) * game_nb_max_bridges(g))
	    {
	      for(int k = 0; k < game_nb_max_bridges(g); k++)
	        add_bridge_dir(g, i, j);
	    }
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




/*
  Applique la possibilité donnée par tab sur le node n dans le game g
*/
 
game apply_possibility(int * tab, game g, int node_num){
  for(int i = 0; i < game_nb_dir(g); i++){
    if(get_degree_dir(g,node_num,i) > tab[i])
      while(get_degree_dir(g,node_num,i) > tab[i]) {del_bridge_dir(g,node_num,i);}

    else if (get_degree_dir(g,node_num,i) < tab[i])
      while(get_degree_dir(g,node_num,i) < tab[i]) {
	if(can_add_bridge_dir(g, node_num, i))
	  add_bridge_dir(g, node_num, i);
      }
  }
  return g;
}

/*
  Fonction utilisée par create_possibilities pour générer les variantes de tab
  La fonction génère la variante du tableau dans l'ordre lexicographique
  Plus de détail : https://www.nayuki.io/page/next-lexicographical-permutation-algorithm
  Le probleme est les doublons générés lorsque deux nombres sont identiques...
*/
static int * permutation (int * tab){
  int * next_tab = malloc(sizeof(*tab));  
  /*generate next_tab*/
  return next_tab;
}

/*
  Retourne une SList
  chaque élément de la liste contient un tableau qui caractérise une possibilté
  exemeple : [1;2;0;0] -> Un pont au nord et deux a l'ouest
 */
SList create_possibilities(game g, int node_num){
  SList list = slist_create_empty();
  /* Générer toute les combinaisons possibles et les stocker dans la liste */

  /*
    Procédure :
     - Comparer le degré requis avec le nombre de pont maximum.
     - En fonction de ça faire une première répartition des points avec un degré maximum dans la répartition à 1, 2 ...
     - Faire une permutation de cette combinaison pour obtenir toute les autres

    Exemple : Si on a un degré requis de 4, un nombre de pont maximum à 2 et 4 directions possible sur 4 alors : 
     -On a la combinaison [1 1 1 1] avec aucune variante. (en faite si avec la fonction permutation lexicographique...)
     -La combinaison [2 1 1 0] et les variantes [1 2 1 0] , [0 1 2 1] ...
*/
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
  
  /* L'indice du noeud à modifier */
  int num = 0;

  /* On boucle jusqu'à la bonne combinaison :) */
  while(!game_over(g)){
    while(slist_next(combinaison[num]) == NULL){
      combinaison[num] = start[num];
      num++;
    }
    g = apply_possibility(slist_data(combinaison[num]),g,num);
  }
  return g;
}

int main(int argc, char *argv[]) {
  if( argc != 2){
    printf("Aucun nom de fichier entré, veuillez écrire :\n./hashi_solve nom_du_fichier\n");
    return EXIT_FAILURE;
  }
  game g = translate_game(argv[1]);
  solver_r(g,0,-1);
  
  char * save = malloc(sizeof(char)*100);
  sprintf(save, "%s.solved",argv[1]);
  write_save(g,save);
  printf("solution found !\n");
  free(save);
  return EXIT_SUCCESS;
}
