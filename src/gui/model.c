// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  // required to load transparent texture from PNG
#include <SDL2/SDL_ttf.h>    // required to use TTF fonts     

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "model.h"
#include "../core/game.h"
#include "../core/node.h"
#include "../core/generate.h"

/* **************************************************************** */


#define FONT "Arial.ttf"

/* **************************************************************** */
     
struct Env_t {
   int * x;
   int * y;
   SDL_Texture ** degree;
   SDL_Texture ** back;
   int size;

  /* PUT YOUR VARIABLES HERE */
}; 
     
/* **************************************************************** */
     
Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[], cgame g)
{  
  Env * env = malloc(sizeof(struct Env_t));

  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  int max = 0;
  int nb_nodes = game_nb_nodes(g);
  

  env->x = malloc(sizeof(int)*nb_nodes);
  env->y = malloc(sizeof(int)*nb_nodes);
  env->degree = malloc(sizeof(SDL_Texture*)*nb_nodes);
  env->back = malloc(sizeof(SDL_Texture*)*nb_nodes);
  

  for(int i = 0 ; i<game_nb_nodes(g);i++){
     node n = game_node(g, i);
     
     if(get_x(n)>max)
        max=get_x(n);
     if(get_y(n)>max)
        max=get_y(n);
  }
  env->size = w/(max *2 + 2);
  
  
  

  

  SDL_Color gray ={150,150,150,255};
  SDL_Color black ={255,255,255,255};

  

  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */
  

  return env;
}
     
/* **************************************************************** */
     
void render(SDL_Window* win, SDL_Renderer* ren, Env * env)
{
  /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */
}
     
/* **************************************************************** */
     
     
bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e)
{     

  if (e->type == SDL_QUIT) {
    return true;
  }

  /* PUT YOUR CODE HERE TO PROCESS EVENTS */
  
  return false; 
}

/* **************************************************************** */

void clean(SDL_Window* win, SDL_Renderer* ren, Env * env)
{
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */

  free(env);
}
     
/* **************************************************************** */
