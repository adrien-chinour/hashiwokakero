// SDL2 Model by aurelien.esnard@u-bordeaux.fr

#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Env_m Envm;

/* **************************************************************** */

#ifdef __ANDROID__
#define PRINT(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__);  } while(0)
#define ERROR(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#else
#define PRINT(STR, ...) do { printf(STR, ##__VA_ARGS__); } while(0)
#define ERROR(STR, ...) do { fprintf(stderr, STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#endif

/* **************************************************************** */

#define APP_NAME "hashiwokakero"
#define DELAY 100

/* **************************************************************** */

Envm * init_menu(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]);
void render_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env);
void clean_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env);
bool process_menu(SDL_Window* win, SDL_Renderer* ren, Envm * env, SDL_Event * e);

/* **************************************************************** */

#endif
