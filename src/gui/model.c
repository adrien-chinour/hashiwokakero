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

/* **************************************************************** */


/* **************************************************************** */

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

Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[], int select) {
  char * game_file = NULL;
  game g = NULL;

  /* L'utilisateur a rentré un nom de fichier */
  if(argc == 2) game_file = argv[1];

  /* On charge le game en fonction de ce que l'utilisateur a choisi dans le menu*/
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
      printf("chargement random\n");
        g = random_game(2,4);
        break;
      case 4:
        //if(check_file("save/game_save.txt"))
        g = SDL_translate_save("save/game_save.txt");
        break;
    }
  }
  if (g == NULL) return NULL;


  /*allocation de notre structure d'environnement*/
  Env * env = malloc(sizeof(struct Env_t));
  if(env == NULL) {delete_game(g); return NULL;} //ERREUR

  env->game_win = NULL;
  env->g = g;

  env->starttime = SDL_GetTicks();

  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  init_window(w,h,ren,env);

  /* Init island texture from PNG image */
  env->island=malloc(sizeof(SDL_Texture*)*game_nb_nodes(g));
  if(env->island == NULL) exit(EXIT_FAILURE); //ERREUR
  env->islandnonselect = IMG_LoadTexture(ren, ISLAND);
  SDL_SetTextureBlendMode(env->islandnonselect,SDL_BLENDMODE_BLEND);
  if(!env->island) SDL_Log("IMG_LoadTexture: %s\n", ISLAND); //ERREUR

  env->solve = IMG_LoadTexture(ren, SOLVE);
  if(!env->solve) SDL_Log("IMG_LoadTexture: %s\n", SOLVE);

  env->save = IMG_LoadTexture(ren, SAVE);
  if(!env->save) SDL_Log("IMG_LoadTexture: %s\n", SAVE);

  env->reload = IMG_LoadTexture(ren, RELOAD);
  if(!env->reload) SDL_Log("IMG_LoadTexture: %s\n", RELOAD);

  env->random = IMG_LoadTexture(ren, RANDOM);
  if(!env->random) SDL_Log("IMG_LoadTexture: %s\n", RANDOM);

  for(int i = 0 ; i < game_nb_nodes(g) ; i++){
    env->island[i] = env->islandnonselect;
  }


  env->islandselect = IMG_LoadTexture(ren,ISLANDSELECT);
  SDL_SetTextureBlendMode(env->islandselect,SDL_BLENDMODE_BLEND);

  SDL_Texture ** text = malloc(sizeof(SDL_Texture*)*game_nb_nodes(g));

  if(text == NULL) {delete_game(g); free(env); return NULL;} //ERREUR //SDLdeletetexture env->island

  env->text = text;

  for(int i = 0 ; i < game_nb_nodes(g) ; i++){
    node n = game_node(g, i);
    SDL_Color color = { 231, 62, 1, 255 };
    TTF_Font * font = TTF_OpenFont(LUNA, env->fontsize);
    if(!font) SDL_Log("TTF_OpenFont: %s\n", LUNA);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    char * degree = malloc(sizeof(char)*10);
    sprintf(degree, "%d", get_required_degree(n));
    SDL_Surface * surf = TTF_RenderText_Blended(font, degree , color);// blended rendering for ultra nice text
    //free(degree);
    env->text[i] = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
  }

  env->node = -1;

  return env;
}

void render(SDL_Window* win, SDL_Renderer* ren, Env * env) {

   SDL_Rect rect;
   int width, height; SDL_GetWindowSize(win, &width, &height);
   SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
   SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_ADD);


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
   SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_MOD);

   print_node_and_bridges(env,ren);

   //timer
   SDL_Color color = {255, 203, 96, 255};
   TTF_Font * font = TTF_OpenFont(LSD, env->fontsize*2);
   if(!font) SDL_Log("TTF_OpenFont: %s\n", LSD);
   TTF_SetFontStyle(font, TTF_STYLE_BOLD);
   char * timer = malloc(sizeof(char)*100);
   int time = SDL_GetTicks() - env->starttime;
   sprintf(timer, "Time : %ds", time/1000);
   SDL_Surface * surf = TTF_RenderText_Blended(font, timer , color);
   SDL_Texture * timer_texture = SDL_CreateTextureFromSurface(ren, surf);
   rect.w = env->size*2;
   rect.h = env->size;
   rect.x = width - rect.w;
   rect.y = height - rect.h;
   SDL_RenderCopy(ren, timer_texture, NULL, &(rect));
   free(timer);
   SDL_FreeSurface(surf);
   TTF_CloseFont(font);

   //solver
   rect.w = env->size/2;
   rect.h = env->size/2;
   rect.x = env->size/4;
   rect.y = height-env->size + env->size/4;
   SDL_RenderCopy(ren, env->solve, NULL, &(rect));

   //save
   rect.w = env->size/2;
   rect.h = env->size/2;
   rect.x = env->size;
   rect.y = height-env->size + env->size/4;
   SDL_RenderCopy(ren, env->save, NULL, &(rect));

   //reload
   rect.w = env->size/2;
   rect.h = env->size/2;
   rect.x = 2 * env->size - env->size/4;
   rect.y = height-env->size + env->size/4;
   SDL_RenderCopy(ren, env->reload, NULL, &(rect));

   //random
   rect.w = env->size/2;
   rect.h = env->size/2;
   rect.x = 2 * env->size + env->size/2;
   rect.y = height-env->size + env->size/4;
   SDL_RenderCopy(ren, env->random, NULL, &(rect));

   if(env->game_win){
      rect.w = env->size*3;
      rect.h = env->size;
      rect.x = width/2 - (rect.w / 2);
      rect.y = height - rect.h*2;
      SDL_RenderCopy(ren, env->game_win, NULL, &(rect));
   }
}

bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* generic events */
  if (e->type == SDL_QUIT) {
    return true;
  }

  if(e->type == SDL_WINDOWEVENT){
    int width, height;
    switch (e->window.event) {
      case SDL_WINDOWEVENT_RESIZED:
      SDL_GetWindowSize(win, &width, &height);
      init_window(width,height,ren,env);
      for(int i = 0; i < game_nb_nodes(env->g); i++){
        print_degree(i, ren, env);
      }
      break;
    }
  }

  /* Android events */

#ifdef __ANDROID__
  else if (game_over(env->g ) && e->type == SDL_FINGERDOWN){
    char ** t = NULL;
    clean(win,ren,env);
    init(win,ren,0,t,0);
  }

  else if (e->type == SDL_FINGERDOWN) {
    button_action(win, ren, env, e->tfinger.x*w, e->tfinger.y*h);
     int node_num = get_node(e->tfinger.x*w, e->tfinger.y*h, env);
     if(node_num !=-1){
        if(env->node == -1){
           env->island[node_num] = env->islandselect;
           env->node = node_num;
        }
        else if(env->node == node_num){
           env->node = -1;
           env->island[node_num] = env->islandnonselect;
        }
        else
           make_connection(node_num, ren, env);
     }
  }
  else if(e->type == SDL_FINGERUP){
     int node_num = get_node(e->tfinger.x*w,e->tfinger.y*h, env);
     make_connection(node_num, ren, env);
  }

  #else
  if(e->type == SDL_MOUSEBUTTONDOWN){
    SDL_Point mousedir;
    SDL_GetMouseState(&mousedir.x, &mousedir.y);

    button_action(win, ren, env, mousedir.x, mousedir.y);

    int node_num = get_node(mousedir.x, mousedir.y, env);
    if(node_num !=-1){
      if(env->node == -1){
        env->island[node_num] = env->islandselect;
        env->node = node_num;
      }
      else if(env->node == node_num){
        env->node = -1;
        env->island[node_num] = env->islandnonselect;
      }
      else
        make_connection(node_num, ren, env);
    }
  }

  else if(e->type == SDL_MOUSEBUTTONUP){
    SDL_Point mousedir;
    SDL_GetMouseState(&mousedir.x, &mousedir.y);
    int node_num = get_node(mousedir.x,mousedir.y, env);
    make_connection(node_num, ren, env);
  }
  else if(e->type == SDL_MOUSEMOTION){
     SDL_GetMouseState(&env->mouse_pos.x, &env->mouse_pos.y);
  }
  #endif

  return false;
}

void clean(SDL_Window* win, SDL_Renderer* ren, Env * env) {
  for(int i = 0; i<game_nb_nodes(env->g);i++){
    SDL_DestroyTexture(env->text[i]);
    SDL_DestroyTexture(env->island[i]);
  }
  SDL_DestroyTexture(env->islandnonselect);
  SDL_DestroyTexture(env->islandselect);
  SDL_DestroyTexture(env->random);
  SDL_DestroyTexture(env->solve);
  SDL_DestroyTexture(env->save);
  SDL_DestroyTexture(env->game_win);
  SDL_DestroyTexture(env->reload);
  delete_game(env->g);
  free(env->island);
  free(env->text);
  free(env);
}
