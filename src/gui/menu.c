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
  int *x; int *y; // coordonnees des planches
  int width_b;
  int height_b;
};

/* **************************************************************** */

Envm * init_menu(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[])
{
  Envm * env = malloc(sizeof(struct Env_m));

  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  env->width_b = w/2;
  env->height_b = h/(NB_BOARDS+1);
  env->fontsize = env->height_b/4;

  env->x = malloc(NB_BOARDS*sizeof(int));
  env->y = malloc(NB_BOARDS*sizeof(int));
  env->text = malloc(NB_BOARDS*sizeof(SDL_Texture*));

  //e = nombre de pixels entre 2 planches (espace)
  int e = h/(2*(NB_BOARDS));

  /* init positions */
  for(int i = 0; i < NB_BOARDS; i++)
    {
      //pour récupérer la taile d'une image int SDL_QueryTexture(env->board, NULL, NULL, X, Y)

      env->x[i] = w/4;
      env->y[i] = (i*(h-e))/NB_BOARDS + e/2;
      int g = env->y[i];
      printf("g = %d\n", g);
    }

  /* INIT TEXTURES */

  /* init background texture from PNG image */
  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if(!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  /* Init board texture from PNG image */
  env->board= IMG_LoadTexture(ren,BOARD);
  if(!env->board) ERROR("IMG_LoadTexture: %s\n", BOARD);

  /* init text texture using Luna font */
  SDL_Color color = { 231, 62, 1, 255 }; /* black color in RGBA */
  TTF_Font * font = TTF_OpenFont(FONT, env->fontsize);
  if(!font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD); // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL
  for(int i = 0; i < NB_BOARDS; i++)
    {

      char * choice;
      switch(i)
	{
	case 0:
	  choice = "jeu facile";
	  break;
	case 1:
	  choice = "jeu moyen";
	  break;
	case 2:
	  choice = "jeu difficile";
	    break;
	case 3:
	  choice = "charger";
	  break;
	}


      SDL_Surface * surf = TTF_RenderText_Blended(font, choice , color);
      env->text[i] = SDL_CreateTextureFromSurface(ren, surf);
      SDL_FreeSurface(surf);
    }

  TTF_CloseFont(font);

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
      rect.x = env->x[i]; rect.y = env->y[i];
      SDL_RenderCopy(ren, env->board, NULL, &rect);

      /* render text texture */
      SDL_QueryTexture(env->text[i], NULL, NULL, &rect.w, &rect.h);
      rect.x = env->x[i]; rect.y = env->y[i];
      SDL_RenderCopy(ren, env->text[i], NULL, &rect);
    }
}

/* **************************************************************** */


bool process_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env, SDL_Event * e)
{

  if (e->type == SDL_QUIT) {
    return true;
  }


  /* PUT YOUR CODE HERE TO PROCESS EVENTS */

  return false;
}

/* **************************************************************** */

void clean_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env)
{
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */

  free(env);
}

/* **************************************************************** */
