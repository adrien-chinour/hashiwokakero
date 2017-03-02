#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../core/node.h"
#include "../core/game.h"

// Les tabeaux utilisés pour généré les games sont déclarés ici.
int game1[7][3] = {{0,0,3},{0,2,5},{0,4,2},{2,2,1},{2,4,2},{4,0,2},{4,4,3}};
int game2[8][3] = {{0,0,3},{0,2,2},{0,4,2},{2,0,2},{2,2,5},{4,0,1},{4,2,1},{4,4,2}};
int game3[7][3] = {{0,0,4},{0,2,2},{0,4,1},{2,2,3},{4,0,5},{4,2,6},{4,4,1}};
int game4[7][3] = {{0,0,4},{0,2,3},{0,4,3},{2,0,6},{2,4,3},{4,0,5},{4,2,4}};
int game5[9][3] = {{0,0,6},{0,2,5},{0,4,3},{2,0,4},{2,2,4},{2,4,6},{4,0,1},{4,2,4},{4,4,5}};
int game6[8][3] = {{0,0,6},{0,2,5},{0,4,4},{2,0,9},{2,2,7},{4,0,4},{4,2,3},{4,4,4}};

// fin déclaration des tableaux.

game generate_game(int nb_game, int nb_max_bridges, int nb_dir, int nb_nodes){
	
    node nodes[nb_nodes];
    for (int i = 0 ; i < nb_nodes; i++){
    	switch(nb_game){
    	  case 1: nodes[i] = new_node(game1[i][0],game1[i][1],game1[i][2]); break;
        case 2: nodes[i] = new_node(game2[i][0],game2[i][1],game2[i][2]); break;
        case 3: nodes[i] = new_node(game3[i][0],game3[i][1],game3[i][2]); break;
        case 4: nodes[i] = new_node(game4[i][0],game4[i][1],game4[i][2]); break;
        case 5: nodes[i] = new_node(game5[i][0],game5[i][1],game5[i][2]); break;
        case 6: nodes[i] = new_node(game6[i][0],game6[i][1],game6[i][2]); break;
        }
    }

    game g = new_game(nb_nodes, nodes, nb_max_bridges, nb_dir);

    for (int i = 0 ; i < nb_nodes; i++) delete_node(nodes[i]);
    
    return g;
}

game translate_game(char * fileopen){
   FILE *gametxt;
   gametxt = fopen(fileopen,"r");
   if(gametxt==NULL){
      fprintf(stderr,"fichier manquant\n");
      return NULL;
   }
   char * l = malloc(sizeof(char)*7);
   fgets(l,7,gametxt);
   int * tab=malloc(sizeof(int)*3);

   char * token = strtok(l," ");
   int tmp;
   int c = 0;

   while(token != NULL){
      tmp=atoi(token);
      tab[c]=tmp;
      token = strtok(NULL, " ");
      c++;
   }
   c=0;
   int * tabnode=malloc(sizeof(int)*3);
   node * nodes= malloc(sizeof(node)*(tab[0]));
   for(int i=0; i<tab[0]; i++){
      fgets(l,7,gametxt);
      char * token = strtok(l," ");
      while(token != NULL){
         tmp=atoi(token);
         tabnode[c]=tmp;
         token = strtok(NULL, " ");
         c++;
      }
      c=0;
      nodes[i]=new_node(tabnode[0], tabnode[1], tabnode[2]);
   }
   game g = new_game(tab[0],nodes,tab[1],tab[2]);

   fclose(gametxt);
   free(tabnode);
   for (int i = 0 ; i < tab[0]; i++) delete_node(nodes[i]);
   free(tab);
   free(l);
   free(nodes);
   return g;
}

game translate_save(char * fileopen){ //pour les sauvegardes + solutions
   FILE *gametxt;
   gametxt = fopen(fileopen,"r");
   if(gametxt==NULL){
      fprintf(stderr,"fichier manquant\n");
   }
   char * l = malloc(sizeof(char)*16);
   fgets(l,10,gametxt);
   int * tab = malloc(sizeof(int)*3);

   char * token = strtok(l, " ");
   int tmp;
   int c = 0;

   while(token != NULL){
      tmp=atoi(token);
      tab[c]=tmp;
      token = strtok(NULL, " ");
      c++;
   }
   c=0;
   int * tabnode = malloc(sizeof(int)*(3+tab[1]));
   node * nodes = malloc(sizeof(node)*(tab[0]));
   
   for(int i=0; i<tab[0]; i++){
      fgets(l,17,gametxt);
      char * token = strtok(l," ");
      while(token != NULL){
         tmp=atoi(token);
         tabnode[c]=tmp;
         token = strtok(NULL, " ");
         c++;
      }
      c=0;
      nodes[i]=new_node(tabnode[0], tabnode[1], tabnode[2]);
   }
   game g = new_game(tab[0],nodes,tab[1],tab[2]);
   
   rewind(gametxt);
   
   fgets(l,10,gametxt);
   for(int i=0; i<tab[0]; i++){
      fgets(l,17,gametxt);
      char * token = strtok(l," ");
      while(token != NULL){
         tmp=atoi(token);
         tabnode[c]=tmp;
         token = strtok(NULL, " ");
         c++;
      }
      c=0;
      for (int j= 3;j<(tab[1]+3);j++){
         int num=game_get_node_number(g, tabnode[0], tabnode[1]);
         for(int k=0;k<tabnode[j];k++){
            add_bridge_dir(g, num,(j-3));
         }
      }
   }

   fclose(gametxt);
   //free(tabnode);
   for (int i = 0 ; i < tab[0]; i++) delete_node(nodes[i]);
   free(tab);
   free(l);
   //free(nodes);
   return g;
}

void write_save(game g, char * filesave){
   FILE *gametxt;
   gametxt = fopen(filesave,"w");
   fprintf(gametxt,"%d %d %d\n",game_nb_nodes(g),game_nb_max_bridges(g),game_nb_dir(g));
   int nb_dir=game_nb_dir(g);
   if(nb_dir==4){
      for(int i=0; i<game_nb_nodes(g);i++){
         node node_i = game_node(g,i);
         fprintf(gametxt,"%d %d %d %d %d %d %d\n",get_x(node_i),get_y(node_i),get_required_degree(node_i),get_degree_dir(g, i, 0),get_degree_dir(g, i, 1),get_degree_dir(g, i, 2),get_degree_dir(g, i, 3));
      }
   }
   else{
      for(int i=0; i<game_nb_nodes(g);i++){
         node node_i = game_node(g,i);
         fprintf(gametxt,"%d %d %d %d %d %d %d %d %d %d %d\n",get_x(node_i),get_y(node_i),get_required_degree(node_i),get_degree_dir(g, i, 0),get_degree_dir(g, i, 1),get_degree_dir(g, i, 2),get_degree_dir(g, i, 3),get_degree_dir(g, i, 4),get_degree_dir(g, i, 5),get_degree_dir(g, i, 6),get_degree_dir(g, i, 7));
      }
   }
   fclose(gametxt);
}

void write_game(game g){
   FILE *gametxt;
   gametxt = fopen("./map/game_1.txt","w");
   fprintf(gametxt,"%d %d %d\n",game_nb_nodes(g),game_nb_max_bridges(g),game_nb_dir(g));
   for(int i=0; i<game_nb_nodes(g);i++){
      node node_i = game_node(g,i);
      fprintf(gametxt,"%d %d %d\n",get_x(node_i),get_y(node_i),get_required_degree(node_i));
   }
}

