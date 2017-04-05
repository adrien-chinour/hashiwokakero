#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "menu.h"

/* **************************************************************** */

#define FONT "Luna.ttf"
#define FONTSIZE 36
#define BACKGROUND "img/background.png"
#define BOARD "img/board.png"
#define NB_BOARDS 4

/* **************************************************************** */

struct Env_m {
   SDL_Texture * background;
   SDL_Texture * board; // texture planche
   SDL_Texture ** text; // texture texte (degré)
   int fontsize; // taille de la police (degré)
   int x; int *y; // coordonnees des planches
   int width_b;
   int height_b;
};

/* **************************************************************** */


void init_window_menu(int w, int h, SDL_Renderer* ren, Envm * env){
   env->width_b = w/2;
   env->height_b = h/(NB_BOARDS+1);

   // definition de la taille de la police
   env->fontsize = env->height_b;
   env->y = malloc(NB_BOARDS*sizeof(int));
   env->text = malloc(NB_BOARDS*sizeof(SDL_Texture*));

   int marge = h/(2*(NB_BOARDS));
   char ** board_message = malloc(sizeof(char*)*NB_BOARDS);

   board_message[0] = "Partie 4dir";
   board_message[1] = "Partie 8dir";
   board_message[2] = "Partie Aleatoire";
   board_message[3] = "Partie sauvegardee";
   /* init text texture using Luna font */
   SDL_Color color = { 231, 62, 1, 255 }; /* black color in RGBA */
   TTF_Font * font = TTF_OpenFont(FONT, env->fontsize);
   if(!font) ERROR("TTF_OpenFont: %s\n", FONT);
   TTF_SetFontStyle(font, TTF_STYLE_NORMAL); // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL

   for(int i = 0; i < NB_BOARDS; i++){
      SDL_Surface * surf = TTF_RenderText_Blended(font, board_message[i] , color);
      env->text[i] = SDL_CreateTextureFromSurface(ren, surf);
      SDL_FreeSurface(surf);
   }


   for(int i = 0; i < NB_BOARDS; i++){
     //positionner les images au centre en abscisse
      env->x = w/4;
      //e = nombre de pixels entre 2 planches (espace)
      env->y[i] = (i*(h-marge))/NB_BOARDS + marge/2;
   }
   TTF_CloseFont(font);
   free(board_message);
}

Envm * init_menu(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]){
   Envm * env = malloc(sizeof(struct Env_m));

   /* get current window size */
   int w, h;
   SDL_GetWindowSize(win, &w, &h);

   init_window_menu(w,h,ren,env);




   /* init positions */





   /* INIT TEXTURES */

   /* init background texture from PNG image */
   env->background = IMG_LoadTexture(ren, BACKGROUND);
   if(!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

   /* Init board texture from PNG image */
   env->board= IMG_LoadTexture(ren,BOARD);
   if(!env->board) ERROR("IMG_LoadTexture: %s\n", BOARD);

   return env;
}

/* **************************************************************** */

void render_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env)
{
   /* RENDER TEXTURES */

   SDL_Rect rect;

   /* get current window size */
   int w, h;
   SDL_GetWindowSize(win, &w, &h);

   /* render background texture */
   SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */

   for(int i = 0; i < NB_BOARDS; i++){
      /* render board texture */
      rect.w = env->width_b;
      rect.h = env->height_b;
      rect.x = env->x; rect.y = env->y[i];
      SDL_RenderCopy(ren, env->board, NULL, &rect);

      /* render text texture */
      //SDL_QueryTexture(env->text[i], NULL, NULL, &rect.w, &rect.h);
      rect.w = rect.w/2; rect.h = rect.h/2;
      rect.x = env->x + env->width_b /4; rect.y = env->y[i] +env->height_b/3;
      SDL_RenderCopy(ren, env->text[i], NULL, &rect);
   }
}


int select_button(Envm * env, int x, int y){
  if(env->x <= x && x <= (env->x +env->width_b)){
    for(int i =0;i<NB_BOARDS;i++){
      if(env->y[i] <= y && y <= (env->y[i] + env->height_b))
        return i+1;
    }
  }
  return 0;
}
/* **************************************************************** */


int process_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env, SDL_Event * e){
  int w,h;
  SDL_GetWindowSize(win, &w, &h);
  if (e->type == SDL_QUIT) {
    return -1;
  }
  else if(e->type == SDL_WINDOWEVENT){
    switch(e->window.event){
    case SDL_WINDOWEVENT_RESIZED:
      init_window_menu(w,h,ren,env);
    }
  }
  /* Android events */

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

  /* PUT YOUR CODE HERE TO PROCESS EVENTS */

  return 0;
}

/* **************************************************************** */

void clean_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env)
{
   /* PUT YOUR CODE HERE TO CLEAN MEMORY */
   SDL_DestroyTexture(env->board);
   SDL_DestroyTexture(env->background);
   for(int i = 0; i<NB_BOARDS;i++){
      SDL_DestroyTexture(env->text[i]);
   }
   free(env->text);
   free(env);
}


/* **************************************************************** */
