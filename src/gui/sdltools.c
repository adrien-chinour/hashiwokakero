#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "model.h"
#include "sdlfile.h"
#include "../core/game.h"
#include "../core/node.h"
#include "../tools/file.h"
#include "../tools/generate.h"

struct Env_t {
  SDL_Texture ** island; // texture ile
  SDL_Texture ** text; // texture texte (degré)
  SDL_Texture * islandnonselect;
  SDL_Texture * islandselect;
  int fontsize; // taille de la police (degré)
  int max_x; int max_y; // coordonnees maxmum des iles
  int margin_x; int margin_y; // marge pour centrer la partie
  game g; // la partie a afficher
  int node; // le noeud selectionné (evenement)
  int size; // taille d'une ile
  SDL_Texture * game_win;
  SDL_Texture * solve;
  SDL_Texture * save;
  SDL_Texture * reload;
  SDL_Texture * random;
  SDL_Point mouse_pos;
  unsigned int starttime;
};



/*
  détruit les variables liées à un objet game
*/
void clean_game(Env* env){
   for(int i = 0; i<game_nb_nodes(env->g);i++){
      SDL_DestroyTexture(env->text[i]);
   }
   delete_game(env->g);
}

/*
  permet de changer de partie
*/
void change_game(SDL_Window* win,Env* env,SDL_Renderer * ren, int num_game){
   int width, height; SDL_GetWindowSize(win, &width, &height);
   clean_game(env);
   if(env->game_win != NULL){SDL_DestroyTexture(env->game_win); env->game_win =NULL;}
   free(env->island);
   free(env->text);
   SDL_SetTextureBlendMode(env->save,SDL_BLENDMODE_NONE);
   init_game(env,NULL,num_game);
   init_window(width,height,ren,env);
   refresh_window(win, ren, env);
}

/*
  initialise les variables liées à un objet game + le game lui même
*/
void init_game(Env * env,char * game_file,int select){
   game g = NULL;
   if(game_file != NULL) {g = translate_game(game_file); }
   else {
      switch (select) {
         case 1:
            g = SDL_translate_game("save/game_default.txt");
            break;
         case 2:
            g = SDL_translate_game("save/3bridges.txt");
            break;
         case 3:
            g = random_game(2,4);
            break;
         case 4:
            g = SDL_translate_save("save/game_save.txt");
            break;
      }
   }
   if (g == NULL){ fprintf(stderr,"problème d'initialisation game\n"); exit(EXIT_FAILURE);}

   env->game_win = NULL;
   env->g = g;
   env->island=malloc(sizeof(SDL_Texture*)*game_nb_nodes(g));
   if(env->island == NULL) exit(EXIT_FAILURE); //ERREUR
   for(int i = 0 ; i < game_nb_nodes(g) ; i++){
      env->island[i] = env->islandnonselect;
   }
   SDL_Texture ** text = malloc(sizeof(SDL_Texture*)*game_nb_nodes(g));
   if(text == NULL) exit(EXIT_FAILURE);
   env->text = text;
   env->node = -1;
}

/*
  permet de "rafraichir" la fenêtre (sert surtout quand on charge une partie différente)
*/
void refresh_window(SDL_Window* win, SDL_Renderer* ren, Env * env){
   int width, height;
   SDL_GetWindowSize(win, &width, &height);
   for(int i = 0; i < game_nb_nodes(env->g); i++){
      print_degree(i, ren, env);
   }
   init_window(width,height,ren,env);
}

/*
  initialise les degrées et leur couleur
*/
void print_degree(int node_num, SDL_Renderer* ren,  Env * env){
  SDL_Surface * surf;
  SDL_Color color_menthe = { 22,184,78,255 }; //couleur menthe
  SDL_Color color_corail = { 231,62,1,255 }; //couleur corail
  node n = game_node(env->g, node_num);
  TTF_Font * font = TTF_OpenFont(LUNA, env->fontsize);
  if(!font) SDL_Log("TTF_OpenFont: %s\n", LUNA);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD);
  char * degree = malloc(sizeof(char)*10);
  sprintf(degree, "%d", get_required_degree(n));
  if(get_required_degree(game_node(env->g, node_num)) == get_degree(env->g, node_num))
    surf = TTF_RenderText_Blended(font, degree , color_menthe);
  else
    surf = TTF_RenderText_Blended(font, degree , color_corail);
  env->text[node_num] = SDL_CreateTextureFromSurface(ren, surf);
  SDL_FreeSurface(surf);
  TTF_CloseFont(font);
}

/*
  affiche les ponts possible (utile que dans la version ordinateur)
*/
void print_bridges(SDL_Renderer* ren ,Env * env){
   SDL_Rect rect;
   int node_pos = get_node(env->mouse_pos.x,env->mouse_pos.y,env);
   int x =0;int y =0;int x1 =0;int y1 =0;

   if(node_pos!=-1){
      SDL_SetRenderDrawColor(ren,100,100,100,SDL_ALPHA_OPAQUE);
      for(int d =0; d<game_nb_dir(env->g); d++){
         if(can_add_bridge_dir(env->g,node_pos, d)){
            node debut = game_node(env->g,node_pos);
            node cible = game_node(env->g,get_neighbour_dir(env->g, node_pos, d));
            switch(d){
               case NORTH:
                  x = coordtopxx(get_x(debut),env)+env->size/2;
                  y = coordtopxy(get_y(debut),env)+env->size;
                  x1 = coordtopxx(get_x(cible),env)+env->size/2;
                  y1 = coordtopxy(get_y(cible),env);
                  rect.x = coordtopxx(get_x(debut),env)+env->size/4;
                  rect.y = coordtopxy(get_y(debut),env)+env->size;
                  rect.h = y1 -y;
                  rect.w = env->size/2;
                  break;
               case WEST:
                  x = coordtopxx(get_x(debut),env);
                  y = coordtopxy(get_y(debut),env)+env->size/2;
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 = coordtopxy(get_y(cible),env)+env->size/2;
                  rect.x = coordtopxx(get_x(debut),env);
                  rect.y = coordtopxy(get_y(debut),env) + env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 - x;
                  break;
               case SOUTH:
                  x = coordtopxx(get_x(debut),env)+env->size/2;
                  y = coordtopxy(get_y(debut),env);
                  x1 = coordtopxx(get_x(cible),env)+env->size/2;
                  y1 =coordtopxy(get_y(cible),env)+env->size;
                  rect.x = coordtopxx(get_x(debut),env)+env->size/4;
                  rect.y = coordtopxy(get_y(debut),env);
                  rect.h = y1 - y;
                  rect.w = env->size/2;
                  break;
               case EAST:
                  x = coordtopxx(get_x(debut),env)+env->size;
                  y = coordtopxy(get_y(debut),env)+env->size/2;
                  x1 = coordtopxx(get_x(cible),env);
                  y1 = coordtopxy(get_y(cible),env)+env->size/2;
                  rect.x = coordtopxx(get_x(debut),env)+env->size;
                  rect.y = coordtopxy(get_y(debut),env)+env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 -x;
                  break;
               case NW:
                  x = coordtopxx(get_x(debut),env);
                  y = coordtopxy(get_y(debut),env)+env->size;
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 =coordtopxy(get_y(cible),env);
                  rect.x = coordtopxx(get_x(debut),env)+env->size;
                  rect.y = coordtopxy(get_y(debut),env)+env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 -x;
                  break;
               case SW:
                  x = coordtopxx(get_x(debut),env);
                  y = coordtopxy(get_y(debut),env);
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 = coordtopxy(get_y(cible),env)+env->size;
                  rect.x = coordtopxx(get_x(debut),env)+env->size;
                  rect.y = coordtopxy(get_y(debut),env)+env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 -x;
                  break;
               case SE:
                  x = coordtopxx(get_x(debut),env)+env->size;
                  y = coordtopxy(get_y(debut),env);
                  x1 = coordtopxx(get_x(cible),env);
                  y1 =coordtopxy(get_y(cible),env)+env->size;
                  rect.x = coordtopxx(get_x(debut),env)+env->size;
                  rect.y = coordtopxy(get_y(debut),env)+env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 -x;
                  break;
               case NE:
                  x = coordtopxx(get_x(debut),env)+env->size;
                  y = coordtopxy(get_y(debut),env)+env->size;
                  x1 = coordtopxx(get_x(cible),env);
                  y1 = coordtopxy(get_y(cible),env);
                  rect.x = coordtopxx(get_x(debut),env)+env->size;
                  rect.y = coordtopxy(get_y(debut),env)+env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 -x;
                  break;
            }
            if(game_nb_dir(env->g) == 4){
               SDL_RenderFillRect(ren,&rect);
               SDL_RenderDrawRect(ren,&rect);
            }
            else
               SDL_RenderDrawLine(ren, x,y, x1, y1);
         }
      }
   }
}

/*
Initialisation des variables d'env en fonction de la taille de l'écran
*/
void init_window(int w, int h, SDL_Renderer* ren, Env * env){
  int max_x = 0; int max_y = 0;
  int margin_x = 0; int margin_y = 0;

  // recupération de max_x et max_y
  for(int i = 0 ; i < game_nb_nodes(env->g) ; i++){
    node n = game_node(env->g, i);
    if(get_x(n) > max_x) max_x = get_x(n);
    if(get_y(n) > max_y) max_y = get_y(n);
  }
  env->max_x = max_x + 1; env->max_y = max_y + 2;

  // definiton de la marge
  if(env->max_x * h > env->max_y * w){
    env->size = w /(env->max_x * 2);
    margin_y = (h  - env->size * (env->max_y * 2)) / 2;
  }
  else {
    env->size = h /(env->max_y * 2);
    margin_x = (w  - env->size * (env->max_x * 2)) / 2;
  }
  env->margin_x = margin_x; env->margin_y = margin_y;

  // definition de la taille de la police
  env->fontsize = env->size/6;
}

/*
  conversion coordonnees vers pixel
*/
int coordtopxx(int coord, Env * env){
  return (coord * 2 + 1) * env->size  - env->size / 2 + env->margin_x;
}
int coordtopxy(int coord, Env * env){
  return (coord * 2 + 1) * env->size  - env->size / 2 + env->margin_y;
}

/*
  Affichage du message de fin a l'ecran
*/
void game_finish(Env * env, SDL_Renderer * ren){
  //note pour plus tard: pour avoir le dernier temps il faut mettre le temps dans la variable d'environnement ou en faire un variable globale
  if(game_over(env->g)){
    SDL_Color color = {255, 203, 96, 255};
    TTF_Font * font = TTF_OpenFont(UBUNTU, env->fontsize*4);
    if(!font) ERROR("TTF_OpenFont: %s\n", UBUNTU);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    unsigned int timer = (SDL_GetTicks()-env->starttime)/1000;
    char end_message[100]; sprintf(end_message, "Victoire ! Temps : %ds", timer);
    SDL_Surface * surf = TTF_RenderText_Blended(font, end_message, color);
    SDL_Texture * end_texture = SDL_CreateTextureFromSurface(ren, surf);
    env->game_win = end_texture;
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
  }
}

/*
  recuperation du numero du noeud a partir des coordonnees d'un pixel
*/
int get_node(int x, int y, Env * env){
  for(int i = 0; i < game_nb_nodes(env->g); i++){
    node n = game_node(env->g, i);
    int coordX = coordtopxx(get_x(n), env);
    int coordY = coordtopxy(get_y(n), env);
    if(coordX < x && coordX+env->size > x && coordY < y && coordY+env->size > y){
      return i;
    }
  }
  return -1;
}

/*
  gestion des ponts sur sdl
*/
void make_connection(int node_num, SDL_Renderer * ren, Env * env){
  if(env->node != -1 && node_num != -1){
    for(int i = 0; i < game_nb_dir(env->g); i++){
      if(get_neighbour_dir(env->g, node_num, i) == env->node){
        if(can_add_bridge_dir(env->g, node_num, i)){
          add_bridge_dir(env->g, node_num, i);
          env->island[node_num]=env->islandnonselect;
          print_degree(node_num, ren, env);
          game_finish(env,ren);
          SDL_SetTextureBlendMode(env->save,SDL_BLENDMODE_NONE);
        }
        else {//supression des ponts
          while(get_degree_dir(env->g, node_num, i) != 0){
            env->island[get_neighbour_dir(env->g, node_num, i)] = env->islandnonselect;
            env->island[node_num]=env->islandnonselect;
            del_bridge_dir(env->g, node_num, i);
            print_degree(node_num, ren, env);
            SDL_SetTextureBlendMode(env->save,SDL_BLENDMODE_NONE);
          }
        }
        print_degree(env->node, ren ,env);
        env->island[env->node] = env->islandnonselect;
        env->node = -1;
        break;
      }
    }
  }
}

/*
  affichage des noeud et des ponts
*/
void print_node_and_bridges(Env * env, SDL_Renderer * ren){
   SDL_Rect rect;
   int x,y,x1,y1;
   int dx =0,dy=0;
   /* nodes */
   for(int i = 0; i < game_nb_nodes(env->g); i++){
      node n = game_node(env->g, i);
      /* texture */
      rect.w = env->size;
      rect.h = env->size;
      rect.x = coordtopxx(get_x(n),env);
      rect.y = coordtopxy(get_y(n),env);
      SDL_RenderCopy(ren, env->island[i], NULL, &(rect));

      /* degree */
      SDL_QueryTexture(env->text[i], NULL, NULL, &rect.w, &rect.h);
      rect.x = coordtopxx(get_x(n),env) + env->size /1.5;
      rect.y = coordtopxy(get_y(n),env) + env->size /1.5;
      SDL_RenderCopy(ren, env->text[i], NULL, &rect);

      if(get_degree(env->g, i)){
         for(int d=0;d<game_nb_dir(env->g);d++){
            if(get_degree_dir(env->g, i, d)>0){
               node cible = game_node(env->g,get_neighbour_dir(env->g, i, d));
               if(d == NORTH){
                  x = coordtopxx(get_x(n),env)+env->size/2;
                  y = coordtopxy(get_y(n),env)+env->size;
                  x1 = coordtopxx(get_x(cible),env)+env->size/2;
                  y1 =coordtopxy(get_y(cible),env);
                  dx =env->size/10;
                  dy =0;
                  if(can_add_bridge_dir(env->g, i, d)){
                     SDL_SetRenderDrawColor(ren, 255, 203, 96, SDL_ALPHA_OPAQUE);
                  }
                  else
                     SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
                  render_bridges(x,y,x1,y1,dx,dy,i,d,env,ren);
               }
               else if(d == WEST){
                  x = coordtopxx(get_x(n),env);
                  y = coordtopxy(get_y(n),env)+env->size/2;
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 =coordtopxy(get_y(cible),env)+env->size/2;
                  dx =0;
                  dy =env->size/10 ;
                  if(can_add_bridge_dir(env->g, i, d)){
                     SDL_SetRenderDrawColor(ren, 255, 203, 96, SDL_ALPHA_OPAQUE);
                  }
                  else
                     SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
                  render_bridges(x,y,x1,y1,dx,dy,i,d,env,ren);
               }
               else if(d == NW){
                  x = coordtopxx(get_x(n),env);
                  y = coordtopxy(get_y(n),env)+env->size;
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 =coordtopxy(get_y(cible),env);
                  dx =env->size/10;
                  dy =env->size/10 ;
                  if(can_add_bridge_dir(env->g, i, d)){
                     SDL_SetRenderDrawColor(ren, 255, 203, 96, SDL_ALPHA_OPAQUE);
                  }
                  else
                     SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
                  render_bridges(x,y,x1,y1,dx,dy,i,d,env,ren);
               }
               else if(d == SW){
                  x = coordtopxx(get_x(n),env);
                  y = coordtopxy(get_y(n),env);
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 =coordtopxy(get_y(cible),env)+env->size;
                  dx =-env->size/10;
                  dy =env->size/10 ;
                  if(can_add_bridge_dir(env->g, i, d)){
                     SDL_SetRenderDrawColor(ren, 255, 203, 96, SDL_ALPHA_OPAQUE);
                  }
                  else
                     SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
                  render_bridges(x,y,x1,y1,dx,dy,i,d,env,ren);
               }
            }
         }
      }
   }
}

/*
  affiche les ponts dans le render
 */
void render_bridges(int x, int y, int x1, int y1, int dx, int dy, int node_num,int dir, Env * env, SDL_Renderer * ren){
   int degree = get_degree_dir(env->g, node_num, dir);
   x = x+(dx*degree)/2;
   y = y+(dy*degree)/2;
   x1 = x1+(dx*degree)/2;
   y1 = y1+(dy*degree)/2;
   for(int j =0; j<get_degree_dir(env->g, node_num, dir);j++){
      SDL_RenderDrawLine(ren, x, y, x1, y1);
      x = x-dx;
      y = y-dy;
      x1 = x1-dx;
      y1 = y1-dy;
   }
}

/*
  gestion des evenements liee au bouton d'action (save,reload,solve...)
*/
void button_action(SDL_Window* win, SDL_Renderer* ren, Env * env, int x , int y){

  int width, height;
  SDL_GetWindowSize(win, &width, &height);

  if(y > height-env->size+env->size/4 && y < height-env->size+env->size/4+env->size/2){
    if(x > env->size/4 && x < env->size/4+env->size/2){
      for(int i = 0; i < game_nb_nodes(env->g); i++){
        for(int j = 0; j < game_nb_dir(env->g); j++){
          while(get_degree_dir(env->g, i, j) != 0){
            del_bridge_dir(env->g, i, j);
          }
        }
      }
      simple_bridges(env->g);
      bool * go=malloc(sizeof(bool));
      *go=false;
      solver_r(env->g,0,-1,go);
      free(go);
      for(int i = 0; i < game_nb_nodes(env->g); i++){
         print_degree(i, ren,  env);
      }
      game_finish(env, ren);
    }
    else if (x >env->size && x < env->size+env->size/2){
       SDL_BlendMode* BM = NULL;
       SDL_GetTextureBlendMode(env->save,BM);
       if(BM == SDL_BLENDMODE_NONE){
          SDL_write_save(env->g, "save/game_save.txt");
          SDL_SetTextureBlendMode(env->save,SDL_BLENDMODE_MOD);
       }
    }
    else if(x > 2 * env->size - env->size/4 && x < 2 * env->size - env->size/4 + env->size/2){
      for(int i = 0; i < game_nb_nodes(env->g); i++){
        for(int j = 0; j < game_nb_dir(env->g); j++){
          if(get_degree_dir(env->g,i,j) != 0)
            del_bridge_dir(env->g,i,j);
        }
      }
      for(int i = 0; i < game_nb_nodes(env->g); i++){
         print_degree(i, ren,  env);
      }
      env->game_win = NULL;
    }
    else if(x > 2 * env->size + env->size/2 && x < 3 * env->size){
       change_game(win, env, ren, 3);
       env->starttime = SDL_GetTicks();
    }
  }
}
