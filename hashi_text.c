#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "node.h"
#include "game.h"

//#define size 20// dimension de la partie



int main(int argc ,char *argv[]) {
   
	printf("\n");

	if(argc != 3){
		printf("Erreur dans le nombre d'argument.\nSynthaxe : ./hashi_text TAILLE NBNODE\n");
		return EXIT_FAILURE;
	}

	int size = atoi(argv[1]);

   	int nbnode = atoi(argv[2]);
   
   	/* Creation d'un tableau de nodes random de size nbnode */
   	node t[nbnode];

   	srand(time(NULL));

   	for(int i = 0; i < nbnode; i++){
   		int r_x = rand()% size;
   		int r_y = rand()% size;
   		int r_d = rand()% 8 + 1;
   		t[i]=new_node(r_x , r_y , r_d);
   	}

   	/* Creation d'une partie à partir des nodes precedement crees */
   	new_game (nbnode,t);

   	/* Affichage des nodes*/

   	printf("Affichage d'une partie de taille %dx%d:\n",size,size);

	
	for(int i = 0; i < size; i++){

		for(int j = 0; j < size; j++){

			bool nodeOk = false; //pour savoir si un node a était détecté

			for(int k = 0; k < nbnode; k++){

				if(get_y(t[k]) == j && get_x(t[k]) == i){
				  
					printf("*%d*",get_required_degree(t[k]));
				        nodeOk = true;
					break;
				}

			}

			if(!nodeOk)
			    printf("   ");
		}
		printf("|\n");
	}

	printf("-- fin de la zone de jeu --\n");

	return EXIT_SUCCESS;
}
