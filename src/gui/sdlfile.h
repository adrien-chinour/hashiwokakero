// SDL2 Model by aurelien.esnard@u-bordeaux.fr

#ifndef SDLFILE_H
#define SDLFILE_H

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "model.h"
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

game SDL_translate_game(char * fileopen);
game SDL_translate_save(char * fileopen);
void SDL_write_save(cgame g, char * filesave);

/* **************************************************************** */

#endif
