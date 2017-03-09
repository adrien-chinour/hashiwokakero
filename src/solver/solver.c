#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../core/node.h"
#include "../core/game.h"
#include "../core/file.h"

bool solver_r(game g,int node_num,int dir,bool * go){
  if(get_required_degree(game_node(g,node_num))==get_degree(g,node_num)){
    if(node_num<(game_nb_nodes(g)-1)){
       solver_r(g,node_num+1,dir,go);
    }
    else{
      if(game_over(g)){
         *go= true;
         return go;
      }
      else{
         del_bridge_dir(g,node_num,dir);
      }
    }
  }
  else{
    for(int d=0; d<game_nb_dir(g);d++){
       if(can_add_bridge_dir(g,node_num,d)){
        add_bridge_dir(g,node_num,d);
        solver_r(g,node_num,d,go);
      }
    }
  }
  if(*go)
     return true;
  else
     del_bridge_dir(g, node_num, dir);
  return false;
}

int main(int argc, char *argv[]) {
  if( argc < 2){
    printf("UTILISATION ./hashi_solve <nom_du_fichier> <nom_de_sauvegarde>");
    return EXIT_FAILURE;
  }
  
  game g = translate_game(argv[1]);

  bool * go=malloc(sizeof(bool));
  *go=false;

  if(solver_r(g,0,-1,go))
     printf("Solution trouvé!\n");
  else 
     printf("Aucune solution.\n");
  return EXIT_SUCCESS;

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
