// SDL2 Model by aurelien.esnard@u-bordeaux.fr

#ifndef MODEL_H
#define MODEL_H

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "../core/game.h"
#include "../core/node.h"
#include "../tools/file.h"

typedef struct Env_t Env;

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

static int coordtopxx(int coord, Env * env);
static int coordtopxy(int coord, Env * env);
void print_node_and_boats(Env * env, SDL_Renderer * ren);
void print_node_and_bridges(Env * env, SDL_Renderer * ren);
void render_bridges(int x, int y, int x1, int y1, int dx, int dy, int node_num,int dir, Env * env, SDL_Renderer * ren);
int get_node(int x, int y, Env * env);


/* **************************************************************** */

#endif
