#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "model.h"
#include "sdltools.h"
#include "sdlfile.h"
#include "../core/game.h"
#include "../core/node.h"
#include "../tools/file.h"
#include "../tools/generate.h"

/*à enlever*/
game SDL_translate_game(char * fileopen){ //inspiré de la documentation https://wiki.libsdl.org/SDL_RWread
   SDL_RWops *file = SDL_RWFromFile(fileopen,"rt");
   if (file == NULL) return NULL;
   Sint64 res_size = SDL_RWsize(file);
   //message d'erreur size

   Sint64 nb_read_total = 0, nb_read = 1;

   char * buf = malloc(sizeof(char));

   buf[0] = '0';
   int * arg = malloc(sizeof(int)*3);
   arg[0]=0;

   /*nb_nodes*/
   int y = 0;
   while(nb_read_total < res_size && nb_read != 0 && buf[0] != ' '){
      y = y +1;
      nb_read_total += nb_read;
      //buf += nb_read;
      arg[0] = arg[0] *10 + atoi(buf);
      nb_read = SDL_RWread(file, buf, sizeof(char),1);
   }

   int c = 1;
   printf("nbnodes = %d\n",arg[0]);

   while(nb_read_total < res_size && nb_read != 0 && buf[0] != '\n'){
      nb_read = SDL_RWread(file, buf, sizeof(char), 1);
      nb_read_total += nb_read;
      if(buf[0] != ' ' && c<3){
         arg[c] = atoi(buf);
         printf("nb other dir et max = %d\n",arg[c]);
         c++;
      }
   }
   c = 0;

   int * node_arg = malloc(sizeof(int)*3);
   node * tab_nodes = malloc(sizeof(node)*(arg[0]));
   for(int i = 0 ; i< arg[0];i++){
      node_arg[0] = 0;
      node_arg[1] = 0;
      node_arg[2] = 0;
      buf[0]='0';
      for(int j = 0; j < 3; j++){
         while(nb_read_total < res_size && nb_read != 0 && buf[0] != ' ' && buf[0] != '\n'){
            node_arg[j] = node_arg[j]*10 +atoi(buf);
            nb_read = SDL_RWread(file, buf, sizeof(char), 1);
            nb_read_total += nb_read;
         }
         buf[0]='0';
         printf("%d ",node_arg[j]);
      }
      printf("\n");
      tab_nodes[i] = new_node(node_arg[0],node_arg[1],node_arg[2]);
   }
   game g = new_game(arg[0],tab_nodes,arg[1],arg[2]);
   printf("nb nodes = %d autres = %d, %d\n",arg[0],arg[1],arg[2]);

   for (int i = 0; i< arg[0];i++) delete_node(tab_nodes[i]);
   free(buf);
   free(arg);
   free(tab_nodes);
   free(node_arg);
   SDL_RWclose(file);
   return g; // après game
}

game SDL_translate_save(char * fileopen){
   SDL_RWops *file = SDL_RWFromFile(fileopen,"rt");
   if (file == NULL) return NULL;
   Sint64 res_size = SDL_RWsize(file);
   //message d'erreur size

   Sint64 nb_read_total = 0, nb_read = 1;

   char * buf = malloc(sizeof(char));

   buf[0] = '0';
   int * arg = malloc(sizeof(int)*3);
   arg[0]=0;

   /*nb_nodes*/
   int y = 0;
   while(nb_read_total < res_size && nb_read != 0 && buf[0] != ' '){
      y = y +1;
      nb_read_total += nb_read;
      //buf += nb_read;
      arg[0] = arg[0] *10 + atoi(buf);
      nb_read = SDL_RWread(file, buf, sizeof(char),1);
   }

   int c = 1;
   printf("nbnodes = %d\n",arg[0]);

   while(nb_read_total < res_size && nb_read != 0 && buf[0] != '\n'){
      nb_read = SDL_RWread(file, buf, sizeof(char), 1);
      nb_read_total += nb_read;
      if(buf[0] != ' ' && c<3){
         arg[c] = atoi(buf);
         printf("nb other dir et max = %d\n",arg[c]);
         c++;
      }
   }
   c = 0;

   int * node_arg = malloc(sizeof(int)*3);
   node * tab_nodes = malloc(sizeof(node)*(arg[0]));
   for(int i = 0 ; i< arg[0];i++){
      node_arg[0] = 0;
      node_arg[1] = 0;
      node_arg[2] = 0;
      buf[0]='0';
      for(int j = 0; j < 3; j++){
         while(nb_read_total < res_size && nb_read != 0 && buf[0] != ' ' && buf[0] != '\n'){//lecture des 3 premiers chiffres
            node_arg[j] = node_arg[j]*10 +atoi(buf);
            nb_read = SDL_RWread(file, buf, sizeof(char), 1);
            nb_read_total += nb_read;
         }
         buf[0]='0';
         printf("%d ",node_arg[j]);
      }
      while(nb_read_total < res_size && nb_read != 0 && buf[0] != '\n'){ //saut de ligne
         nb_read = SDL_RWread(file, buf, sizeof(char), 1);
         nb_read_total += nb_read;
      }
      printf("\n");
      tab_nodes[i] = new_node(node_arg[0],node_arg[1],node_arg[2]);
   }

   game g = new_game(arg[0],tab_nodes,arg[1],arg[2]);
   SDL_RWseek(file,0,RW_SEEK_SET); //retour au début du fichier
   nb_read_total = 0;

   while(nb_read_total < res_size && nb_read != 0 && buf[0] != '\n'){//saut de la première ligne
      nb_read = SDL_RWread(file, buf, sizeof(char), 1);
      nb_read_total += nb_read;
   }

   for(int i = 0 ; i< arg[0];i++){//remplacer i par "node_num"
      buf[0]='0';
      for(int j = 0; j < 3; j++){
         while(nb_read_total < res_size && nb_read != 0 && buf[0] != ' '){//saut des 3 premiers chiffres
            nb_read = SDL_RWread(file, buf, sizeof(char), 1);
            nb_read_total += nb_read;
         }
         buf[0]='0';
      }
      for(int d =0; d < arg[2]; d++){
         buf[0]='0';
         while(nb_read_total < res_size && nb_read != 0 && buf[0] != ' ' && buf[0] != '\n'){
            nb_read = SDL_RWread(file, buf, sizeof(char), 1);
            nb_read_total += nb_read;
            for(int nbbridges = 0; nbbridges<atoi(buf) ; nbbridges++){
               add_bridge_dir(g,i,d);
            }
         }
      }
      if(buf[0] != '\n'){//saut de ligne si ce n'est pas déjà fait
         buf[0]='0';
         while(nb_read_total < res_size && nb_read != 0 && buf[0] != '\n'){//saut de ligne
            nb_read = SDL_RWread(file, buf, sizeof(char), 1);
            nb_read_total += nb_read;
         }
      }
   }


   for (int i = 0; i< arg[0];i++) delete_node(tab_nodes[i]);
   free(buf);
   free(arg);
   free(tab_nodes);
   free(node_arg);
   SDL_RWclose(file);
   return g;
}

void SDL_write_save(game g, char * filesave){
   SDL_RWops *file = SDL_RWFromFile(filesave,"wt");
   if (file == NULL) printf("erreur\n");//erreur ->exit failure
   char* game_txt =malloc(sizeof(char)*1000);
   sprintf(game_txt,"%d %d %d\n",game_nb_nodes(g),game_nb_max_bridges(g),game_nb_dir(g));
   size_t len = SDL_strlen(game_txt);
   if (SDL_RWwrite(file, game_txt, 1, len) != len) {
      SDL_RWclose(file);
      fprintf(stderr,"Couldn't fully write string\n");
      exit(EXIT_FAILURE);
   }
   if(game_nb_dir(g)==4){
      for(int i=0; i<game_nb_nodes(g);i++){
         node node_i = game_node(g,i);
         sprintf(game_txt,"%d %d %d %d %d %d %d\n",get_x(node_i),get_y(node_i),get_required_degree(node_i),get_degree_dir(g, i, 0),get_degree_dir(g, i, 1),get_degree_dir(g, i, 2),get_degree_dir(g, i, 3));
         len = SDL_strlen(game_txt);
         if (SDL_RWwrite(file, game_txt, 1, len) != len) {
            SDL_RWclose(file);
            fprintf(stderr,"Couldn't fully write string\n");
            exit(EXIT_FAILURE);
         }
      }
   }
   else{
      for(int i=0; i<game_nb_nodes(g);i++){
         node node_i = game_node(g,i);
         sprintf(game_txt,"%d %d %d %d %d %d %d %d %d %d %d\n",get_x(node_i),get_y(node_i),get_required_degree(node_i),get_degree_dir(g, i, 0),get_degree_dir(g, i, 1),get_degree_dir(g, i, 2),get_degree_dir(g, i, 3),get_degree_dir(g, i, 4),get_degree_dir(g, i, 5),get_degree_dir(g, i, 6),get_degree_dir(g, i, 7));
         len = SDL_strlen(game_txt);
         if (SDL_RWwrite(file, game_txt, 1, len) != len) {
            SDL_RWclose(file);
            fprintf(stderr,"Couldn't fully write string\n");
            exit(EXIT_FAILURE);
         }
      }
   }
   free(game_txt);
   SDL_RWclose(file);
}
