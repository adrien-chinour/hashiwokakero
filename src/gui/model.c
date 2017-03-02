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
#define FONTSIZE 20
#define ISLAND "img/island.png"
#define BOAT1 "img/boat1.png"
#define BOAT2 "img/boat2.png"
#define BOAT3 "img/boat3.png"
#define BOAT4 "img/boat4.png"

/* **************************************************************** */
     
struct Env_t {
  SDL_Texture * island;
  SDL_Texture * boat1;
  SDL_Texture * boat2;
  SDL_Texture * boat3;
  SDL_Texture * boat4;
  SDL_Texture ** text;
  int max;
  game g;
}; 
     
Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]) {

  char * game_file = NULL;
  game g = NULL;
  
  /* L'utilisateur a rentré un nom de fichier */
  if(argc >= 3) game_file = argv[2];
  if (game_file != NULL) printf("%s\n",game_file); //debug

  /* On charge le game en fonction de ce que l'utilisateur demande*/
  if(game_file != NULL) {g = translate_game(game_file); }
  else {g = translate_game("save/game_default.txt"); }

  /* test retour fonction translate*/
  if (g == NULL) return NULL;

  /*allocation de notre structure d'environnement*/
  Env * env = malloc(sizeof(struct Env_t));
  if(env == NULL) {delete_game(g); return NULL;}

  env->g = g;  
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  int max = 0;

  /* Init island texture from PNG image */
  env->island = IMG_LoadTexture(ren, ISLAND);
  if(!env->island) ERROR("IMG_LoadTexture: %s\n", ISLAND);

  /* Init boat texture from PNG image */
  env->boat1 = IMG_LoadTexture(ren, BOAT1);
  if(!env->boat1) ERROR("IMG_LoadTexture: %s\n", BOAT1);
  env->boat2 = IMG_LoadTexture(ren, BOAT2);
  if(!env->boat2) ERROR("IMG_LoadTexture: %s\n", BOAT2);
  env->boat3 = IMG_LoadTexture(ren, BOAT3);
  if(!env->boat3) ERROR("IMG_LoadTexture: %s\n", BOAT3);
  env->boat4 = IMG_LoadTexture(ren, BOAT4);
  if(!env->boat4) ERROR("IMG_LoadTexture: %s\n", BOAT4);

  
  
  SDL_Texture ** text = malloc(sizeof(SDL_Texture*)*game_nb_nodes(g));
  if(text == NULL) {delete_game(g); free(env); return NULL;}
  
  env->text = text;
  
  for(int i = 0 ; i < game_nb_nodes(g) ; i++){
    node n = game_node(g, i);
    if(get_x(n) > max) max = get_x(n);
    if(get_y(n) > max) max = get_y(n);
    SDL_Color color = { 0, 0, 0, 255 }; /* black color in RGBA */
    TTF_Font * font = TTF_OpenFont(FONT, FONTSIZE);
    if(!font) ERROR("TTF_OpenFont: %s\n", FONT);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    char * degree = malloc(sizeof(char)*10);
    sprintf(degree, "%d", get_required_degree(n));
    SDL_Surface * surf = TTF_RenderText_Blended(font, degree , color); // blended rendering for ultra nice text
    //free(degree);
    env->text[i] = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
  }
  
  env->max = max + 1;

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

  /* nodes */
  for(int i = 0; i < game_nb_nodes(env->g); i++){
    node n = game_node(env->g, i);
    
    /* texture */
    rect.w = size;
    rect.h = size;
    rect.x = (get_x(n) * 2 + 1) * size  - size / 2;
    rect.y = (get_y(n) * 2 + 1) * size - size / 2;
    SDL_RenderCopy(ren, env->island, NULL, &(rect));
    
    /* degree */
    SDL_QueryTexture(env->text[i], NULL, NULL, &rect.w, &rect.h);
    rect.x = (get_x(n) * 2 + 1) * size  - size / 2;
    rect.y = (get_y(n) * 2 + 1) * size  - size / 2;
    SDL_RenderCopy(ren, env->text[i], NULL, &rect);
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
  delete_game(env->g);
  free(env->text);
  free(env);
}
