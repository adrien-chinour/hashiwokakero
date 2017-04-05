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

  /* L'utilisateur a rentré un nom de fichier */
  if(argc == 2) game_file = argv[1];

  /* On charge le game en fonction de ce que l'utilisateur a choisi dans le menu*/

  /*allocation de notre structure d'environnement*/
  Env * env = malloc(sizeof(struct Env_t));
  if(env == NULL) {return NULL;} //ERREUR

  env->starttime = SDL_GetTicks();

  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* Init island texture from PNG image */
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

  env->islandselect = IMG_LoadTexture(ren,ISLANDSELECT);
  SDL_SetTextureBlendMode(env->islandselect,SDL_BLENDMODE_BLEND);

  init_game(env,game_file,select);
  init_window(w,h,ren,env);

  if(env->text == NULL) {delete_game(env->g); free(env); return NULL;} //ERREUR //SDLdeletetexture env->island

  refresh_window(win, ren, env);
  return env;
}

void render(SDL_Window* win, SDL_Renderer* ren, Env * env) {

   SDL_Rect rect;
   int width, height; SDL_GetWindowSize(win, &width, &height);
   SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
   SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_ADD);

   print_bridges(ren,env);
   SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_MOD);

   print_node_and_bridges(env,ren);

   //timer
   SDL_Color color = {255, 203, 96, 255};
   TTF_Font * font = TTF_OpenFont(UBUNTU, env->fontsize*4);
   if(!font) SDL_Log("TTF_OpenFont: %s\n", UBUNTU);
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
    switch (e->window.event) {
      case SDL_WINDOWEVENT_RESIZED:
      break;
    }
    refresh_window(win,ren,env);
  }

  /* Android events */

#ifdef __ANDROID__
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
  SDL_DestroyTexture(env->islandnonselect);
  SDL_DestroyTexture(env->islandselect);
  SDL_DestroyTexture(env->random);
  SDL_DestroyTexture(env->solve);
  SDL_DestroyTexture(env->save);
  SDL_DestroyTexture(env->game_win);
  SDL_DestroyTexture(env->reload);
  clean_game(env);
  free(env->island);
  free(env->text);
  free(env);
}
