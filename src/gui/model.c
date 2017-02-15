#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  // required to load transparent texture from PNG
#include <SDL2/SDL_ttf.h>    // required to use TTF fonts

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "model.h"
#include "../core/game.h"
#include "../core/node.h"
#include "../core/file.h"

/* **************************************************************** */

#define FONT "Arial.ttf"
#define ISLAND "img/island.png"
#define BOAT1 "img/boat1.png"
#define BOAT2 "img/boat2.png"
#define BOAT3 "img/boat3.png"
#define BOAT4 "img/boat4.png"

/* **************************************************************** */
     
struct Env_t {
  int * x;
  int * y;
  SDL_Texture * island;
  SDL_Texture * boat1;
  SDL_Texture * boat2;
  SDL_Texture * boat3;
  SDL_Texture * boat4;
  int max;
  int nb_nodes;
}; 
     
Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]) {

  game g = translate_game();
  
  Env * env = malloc(sizeof(struct Env_t));
  if(env == NULL) return NULL;
  
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  int max = 0;
  int nb_nodes= game_nb_nodes(g);

  
  env->x = malloc(sizeof(int)*nb_nodes);
  if(env->x == NULL) {free(env); return NULL;}
  
  env->y = malloc(sizeof(int)*nb_nodes);
  if(env->y == NULL) {free(env->x); free(env); return NULL;}
  
  env->island = IMG_LoadTexture(ren, ISLAND);
  if(!env->island) ERROR("IMG_LoadTexture: %s\n", ISLAND);

  env->boat1 = IMG_LoadTexture(ren, BOAT1);
  if(!env->boat1) ERROR("IMG_LoadTexture: %s\n", BOAT1);

  env->boat2 = IMG_LoadTexture(ren, BOAT2);
  if(!env->boat2) ERROR("IMG_LoadTexture: %s\n", BOAT2);

  env->boat3 = IMG_LoadTexture(ren, BOAT3);
  if(!env->boat3) ERROR("IMG_LoadTexture: %s\n", BOAT3);

  env->boat4 = IMG_LoadTexture(ren, BOAT4);
  if(!env->boat4) ERROR("IMG_LoadTexture: %s\n", BOAT4);

  for(int i = 0 ; i < nb_nodes ; i++){
     node n = game_node(g, i);
     env->x[i] = get_x(n);
     env->y[i] = get_y(n);
     if(get_x(n) > max) max = get_x(n);
     if(get_y(n) > max) max = get_y(n);
  }
  
  env->max = max + 1;
  env->nb_nodes = nb_nodes;

  
  delete_game(g);
  return env;
}

void render(SDL_Window* win, SDL_Renderer* ren, Env * env) {
  SDL_Rect rect;
  
  int width, height, size;
  SDL_GetWindowSize(win, &width, &height);
  size = width/(env->max * 2);
  
  /* background lines in white */
  SDL_SetRenderDrawColor(ren , 255, 255, 255, 100);
  for(int i = 1; i < env->max * 2; i++){
    SDL_RenderDrawLine(ren, (i * size), 0, (i * size), width);
    SDL_RenderDrawLine(ren, 0, (i * size), height, (i * size));
  }

  /* nodes textures */
  for(int i = 0; i < env->nb_nodes; i++){
    rect.w = size;
    rect.h = size;
    rect.x = (env->x[i] * 2 + 1) * size  - size / 2;
    rect.y = (env->y[i] * 2 + 1) * size - size / 2;
    SDL_RenderCopy(ren, env->island, NULL, &(rect));
  }

  /* test boat texture */
  //boat1
  //rect.w = size/4; rect.h = size/2; rect.x = 10; rect.y = 10;
  //SDL_RenderCopy(ren, env->boat1, NULL, &(rect));
  //boat2
  //rect.w = size/4; rect.h = size/2; rect.x = 10; rect.y = 10;
  //SDL_RenderCopy(ren, env->boat2, NULL, &(rect));
  //boat3
  //rect.w = size/4; rect.h = size/2; rect.x = 10; rect.y = 10;
  //SDL_RenderCopy(ren, env->boat3, NULL, &(rect));
  //boat4
  //rect.w = size/4; rect.h = size/2; rect.x = 10; rect.y = 10;
  //SDL_RenderCopy(ren, env->boat4, NULL, &(rect));
  
}

bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* generic events */
  if (e->type == SDL_QUIT) {
    return true;
  }
  
  return false; 
}

void clean(SDL_Window* win, SDL_Renderer* ren, Env * env) {
  free(env->x);
  free(env->y);
  free(env);
}
