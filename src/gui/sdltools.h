// SDL2 Model by aurelien.esnard@u-bordeaux.fr

#ifndef SDLTOOLS_H
#define SDLTOOLS_H

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "model.h"
#include "sdlfile.h"
#include "../core/game.h"
#include "../core/node.h"
#include "../tools/file.h"

typedef struct Env_t Env;

/* **************************************************************** */

#ifdef __ANDROID__
#define PRINT(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__);  } while(0)
#define ERROR(STR, ...) do { SDL_Log(STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#else
#define PRINT(STR, ...) do { printf(STR, ##__VA_ARGS__); } while(0)
#define ERROR(STR, ...) do { fprintf(stderr, STR, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#endif

/* **************************************************************** */

void change_game(SDL_Window* win,Env* env,SDL_Renderer * ren, int num_game);
void init_game(Env * env,char * game_file,int select);
void refresh_window(SDL_Window* win, SDL_Renderer* ren, Env * env);
int coordtopxx(int coord, Env * env);
int coordtopxy(int coord, Env * env);
int get_node(int x, int y, Env * env);
void game_finish(Env * env, SDL_Renderer * ren);
void print_degree(int node_num, SDL_Renderer* ren,  Env * env);
void init_window(int w, int h, SDL_Renderer* ren, Env * env);
void make_connection(int node_num, SDL_Renderer * ren, Env * env);
void button_action(SDL_Window* win, SDL_Renderer* ren, Env * env, int x , int y);
void print_node_and_bridges(Env * env, SDL_Renderer * ren);
void render_bridges(int x, int y, int x1, int y1, int dx, int dy, int node_num,int dir, Env * env, SDL_Renderer * ren);
int get_node(int x, int y, Env * env);

/* **************************************************************** */

#endif
