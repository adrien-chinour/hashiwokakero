#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "menu.h"

/* **************************************************************** */

#define FONT "fonts/look sir, droids.ttf"
#define FONTSIZE 36
#define BACKGROUND "img/background.png"
#define BOARD "img/board.png"
#define NB_BOARDS 4

/* **************************************************************** */

struct Env_m {
   SDL_Texture * background;  // texture background
   SDL_Texture * board;       // texture planche
   SDL_Texture ** text;       // texture texte (texte des plaches)
   int fontsize;              // taille de la police
   int x; int *y;             // coordonnees des planches
   int width_b;               // longueur planches
   int height_b;              // hauteur planche
};

/* **************************************************************** */

/*
  initialisation de l'environnement relatif a la fenetre
  les variables de l'environnement sont toutes defini en fonction de la taille de la fenetre (w,h)
  difference sur android : les planches sont plus large (bonus : ajouter cette fonctionnalité pour un affichage avec un width petit)
*/
void init_window_menu(int w, int h, SDL_Renderer* ren, Envm * env){

  #ifdef __ANDROID__
  env->width_b = w;
  #else
  env->width_b = w/2;
  #endif

  env->height_b = h/(NB_BOARDS+1);
  env->fontsize = env->height_b;

  //allocation
  env->y = malloc(NB_BOARDS*sizeof(int));
  if(env->y == NULL) ERROR("allocation de env->y dans le menu impossible\n");
  env->text = malloc(NB_BOARDS*sizeof(SDL_Texture*));
  if(env->text == NULL) ERROR("allocation de env->text dans le menu impossible\n");

  //les messages a afficher dans les boards
  char ** board_message = malloc(sizeof(char*)*NB_BOARDS);
  board_message[0] = "Partie 4dir"; board_message[1] = "Partie 8dir";
  board_message[2] = "Aleatoire"; board_message[3] = "Sauvegarde";

  /* init text texture using Luna font */
  SDL_Color color = { 231, 62, 1, 255 };
  TTF_Font * font = TTF_OpenFont(FONT, env->fontsize);
  if(!font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

  //creation des textures a partir du texte
  for(int i = 0; i < NB_BOARDS; i++){
    SDL_Surface * surf = TTF_RenderText_Blended(font, board_message[i] , color);
    env->text[i] = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
  }

  //positionner les images au centre en abscisse
  #ifdef __ANDROID__
  env->x = 0;
  #else
  env->x = w/4;
  #endif

  //positionner les images au centre en ordonnee
  int marge = h/(2*(NB_BOARDS));
  for(int i = 0; i < NB_BOARDS; i++){
    env->y[i] = (i*(h-marge))/NB_BOARDS + marge/2;
  }

  //nettoyage divers
  TTF_CloseFont(font);
  free(board_message);
}


/*
  initialisation de l'environnement de SDL
*/
Envm * init_menu(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]){
   Envm * env = malloc(sizeof(struct Env_m));

   /* get current window size */
   int w, h;
   SDL_GetWindowSize(win, &w, &h);

   /* environnement en fonction de la fenetre , dans une fonction annexe car les variables sont redefini a chaque redimensionnement de fenetre */
   init_window_menu(w,h,ren,env);

   /* init background texture from PNG image */
   env->background = IMG_LoadTexture(ren, BACKGROUND);
   if(!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

   /* Init board texture from PNG image */
   env->board= IMG_LoadTexture(ren,BOARD);
   if(!env->board) ERROR("IMG_LoadTexture: %s\n", BOARD);

   return env;
}

/*
  fonction render_menu ; affichage des elements du menu (board et texte)
*/
void render_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env) {
   SDL_Rect rect; // utiliser pour les tailles des textures et leurs placements

   /* get current window size */
   int w, h;
   SDL_GetWindowSize(win, &w, &h);

   /* render background texture */
   SDL_RenderCopy(ren, env->background, NULL, NULL);

   for(int i = 0; i < NB_BOARDS; i++){
      /* render board texture */
      rect.w = env->width_b; rect.h = env->height_b;
      rect.x = env->x; rect.y = env->y[i];
      SDL_RenderCopy(ren, env->board, NULL, &rect);

      /* render text texture */
      rect.w = rect.w/2; rect.h = rect.h/2;
      rect.x = env->x + env->width_b /4; rect.y = env->y[i] +env->height_b/3;
      SDL_RenderCopy(ren, env->text[i], NULL, &rect);
   }
}

/*
  fonction annexe a process_menu pour recuperer le choix de l'utilisateur en fonction des coordonnees (x,y)
*/
int select_button(Envm * env, int x, int y){
  if(env->x <= x && x <= (env->x +env->width_b)){
    for(int i =0;i<NB_BOARDS;i++){
      if(env->y[i] <= y && y <= (env->y[i] + env->height_b))
        return i+1;
    }
  }
  return 0;
}

/*
  fonction process_menu ; gestion des evenement du menu (clic sur board, redimmensionnement et quitter)
*/
int process_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env, SDL_Event * e){
  int w,h;
  SDL_GetWindowSize(win, &w, &h);

  //evenement quitter
  if (e->type == SDL_QUIT) {
    return -1;
  }

  //evenement redimmensionnement
  else if(e->type == SDL_WINDOWEVENT){
    switch(e->window.event){
    case SDL_WINDOWEVENT_RESIZED:
      init_window_menu(w,h,ren,env);
    }
  }

  //evenement clic
  #ifdef __ANDROID__
  else if(e->type == SDL_FINGERDOWN){
    int selec =select_button(env,e->tfinger.x*w,e->tfinger.y*h);
    if(selec)
      return selec;
  }
  #else
  else if(e->type == SDL_MOUSEBUTTONDOWN){
    SDL_Point mousedir;
    SDL_GetMouseState(&mousedir.x, &mousedir.y);
    int selec =select_button(env,mousedir.x,mousedir.y);
    if(selec)
      return selec;
  }
  #endif

  return 0;
}


/*
  Nettoyage des element de l'environnement du menu (boards, background et text)
*/
void clean_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env) {
   SDL_DestroyTexture(env->board);
   SDL_DestroyTexture(env->background);
   for(int i = 0; i<NB_BOARDS;i++){
      SDL_DestroyTexture(env->text[i]);
   }
   free(env->text);
   free(env);
}


/* **************************************************************** */
