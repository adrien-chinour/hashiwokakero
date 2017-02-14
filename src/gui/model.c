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

/* **************************************************************** */
     
struct Env_t {
  int * x;
  int * y;
  SDL_Texture ** degree;
  SDL_Texture ** back;
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
  if(env->y == NULL) {free(env); return NULL;}
  
  env->degree = malloc(sizeof(SDL_Texture*)*nb_nodes);
  if(env->degree == NULL) {free(env); return NULL;}
  
  env->back = malloc(sizeof(SDL_Texture*)*nb_nodes);
  if(env->back == NULL) {free(env); return NULL;}
  
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

  int width, height, size;
  SDL_GetWindowSize(win, &width, &height);
  size = width/(env->max * 2);
  
  /* background lines in gray */
  SDL_SetRenderDrawColor(ren , 0xA0, 0xA0, 0xA0, SDL_ALPHA_OPAQUE);
  for(int i = 1; i < env->max * 2; i++){
    SDL_RenderDrawLine(ren, (i * size), 0, (i * size), width);
    SDL_RenderDrawLine(ren, 0, (i * size), height, (i * size));
  }

  /* nodes in black */
  SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
  for(int i = 0; i < env->nb_nodes; i++){
    SDL_Rect srcrect;
    srcrect.x = (env->x[i] * 2 + 1) * size  - size / 2;
    srcrect.y = (env->y[i] * 2 + 1) * size - size / 2;
    srcrect.w = size;
    srcrect.h = size;
    SDL_RenderFillRect(ren, &(srcrect));
    SDL_RenderDrawRect(ren, &(srcrect));
  }
  
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

void clean(SDL_Window* win, SDL_Renderer* ren, Env * env)
{
  
  free(env);
}
