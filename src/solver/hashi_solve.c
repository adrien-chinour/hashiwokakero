#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../tools/file.h"
#include "solver.h"

int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("UTILISATION ./hashi_solve <nom_du_fichier> <nom_de_sauvegarde>");
    return EXIT_FAILURE;
  }

  game g = translate_game(argv[1]);

  //auto_play(g, 0, false); //non fonctionnel
  simple_bridges(g);

  bool * go=malloc(sizeof(bool));
  *go=false;

  if(solver_r(g,0,-1,go))
    printf("Solution trouvé!\n");
  else
    printf("Aucune solution.\n");

  char * save = malloc(sizeof(char)*100);
  if(argc > 2)
    write_save(g,argv[2]);
  else {
    sprintf(save, "%s.solved",argv[1]);
    write_save(g,save);
  }

  free(go);
  delete_game(g);
  free(save);
  return EXIT_SUCCESS;
}
