// SDL2 Model by aurelien.esnard@u-bordeaux.fr

#ifndef MODEL_H
#define MODEL_H

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "sdltools.h"
#include "sdlfile.h"
#include "../core/game.h"
#include "../core/node.h"
#include "../tools/file.h"

typedef struct Env_t Env;

#define LUNA "Luna.ttf"
#define ARIAL "Arial.ttf"
#define ISLAND "img/island.png"
#define BOAT1 "img/boat1.png"
#define BOAT2 "img/boat2.png"
#define BOAT3 "img/boat3.png"
#define BOAT4 "img/boat4.png"
#define ISLANDSELECT "img/island_selected.png"
#define SOLVE "img/solve.png"
#define SAVE "img/save.png"
#define RELOAD "img/reload.png"
#define RANDOM "img/random.png"


/* **************************************************************** */

#ifdef __ANDROID__
#define PRINT(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__);  } while(0)
#define ERROR(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
// #define LOG(TAG, STR, ...) __android_log_print(ANDROID_LOG_VERBOSE, TAG ,STR, ##__VA_ARGS__)
# else
#define PRINT(STR, ...) do { printf(STR, ##__VA_ARGS__); } while(0)
#define ERROR(STR, ...) do { fprintf(stderr, STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#endif

/* **************************************************************** */

Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[], int select);
void render(SDL_Window* win, SDL_Renderer* ren, Env * env);
void clean(SDL_Window* win, SDL_Renderer* ren, Env * env);
bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e);

/* **************************************************************** */

#endif
