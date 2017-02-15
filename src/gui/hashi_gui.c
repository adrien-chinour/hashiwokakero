#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  // required to load transparent texture from PNG
#include <SDL2/SDL_ttf.h>    // required to use TTF fonts     
#include <stdio.h>
#include <stdbool.h>
#include "model.h"
     
/* **************************************************************** */
     
#define SCREEN_SIZE 500

/* **************************************************************** */
     
int main(int argc, char * argv[]) {
  
  /* initialize SDL2 and some extensions */
  if(SDL_Init(SDL_INIT_VIDEO) != 0) ERROR("SDL_Init VIDEO"); 
  if(IMG_Init(IMG_INIT_PNG & IMG_INIT_PNG) != IMG_INIT_PNG) ERROR("IMG_Init PNG");
  if(TTF_Init() != 0) ERROR("TTF_Init");  

  int screen_size;
  if(argc > 1 && atoi(argv[1]) > 100 && atoi(argv[1]) < 4000)
    screen_size = atoi(argv[1]);
  else
    screen_size = SCREEN_SIZE;
  
  /* create window and renderer */
  SDL_Window * win = SDL_CreateWindow("Hashiwokakero - TM2H",
				      SDL_WINDOWPOS_UNDEFINED,
				      SDL_WINDOWPOS_UNDEFINED,
				      screen_size, screen_size,
				      SDL_WINDOW_SHOWN);
  if(!win) ERROR("SDL_CreateWindow");  

  SDL_Renderer * ren = SDL_CreateRenderer(win, -1,
					  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(!ren) ERROR("SDL_CreateWindow");

  /* add game menu here */
  //exemple : char * game_file = display_game_menu();
  //return path to the game selected
  
  /* initialize game environment */
  Env * env = init(win, ren, argc, argv);
  if (env == NULL){
    fprintf(stderr, "Alloc (struct Env_t *) failed");
    exit(EXIT_FAILURE);
  }
  
  /* main render loop */
  bool quit = false;
  while (!quit) {
     
    /* manage events */
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      /* process your events */
      quit = process(win, ren, env, &e);
      if(quit) break;
    }
     
    /* background in white */
    SDL_SetRenderDrawColor(ren, 0, 204, 255, SDL_ALPHA_OPAQUE); 
    SDL_RenderClear(ren);
    
    /* render all what you want */
    render(win, ren, env); 
    SDL_RenderPresent(ren);
    SDL_Delay(10);
  }
     
  /* clean your environment */
  clean(win, ren, env);
  
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  TTF_Quit();  
  SDL_Quit();
     
  return EXIT_SUCCESS;
}

