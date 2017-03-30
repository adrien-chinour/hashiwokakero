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

#define FONT "Luna.ttf"
#define ISLAND "img/island.png"
#define BOAT1 "img/boat1.png"
#define BOAT2 "img/boat2.png"
#define BOAT3 "img/boat3.png"
#define BOAT4 "img/boat4.png"
#define ISLANDSELECT "img/island_selected.png"

/* **************************************************************** */

struct Env_t {
   SDL_Texture ** island; // texture ile
   SDL_Texture * boat1; SDL_Texture * boat2; // texture bateau
   SDL_Texture * boat3; SDL_Texture * boat4; //texture bateau
   SDL_Texture ** text; // texture texte (degré)
   SDL_Texture * islandnonselect;
   SDL_Texture * islandselect;
   int fontsize; // taille de la police (degré)
   int max_x; int max_y; // coordonnees maxmum des iles
   int margin_x; int margin_y; // marge pour centrer la partie
   game g; // la partie a afficher
   int node; // le noeud selectionné (evenement)
   int size; // taille d'une ile
};

void print_degree(int node_num, SDL_Renderer* ren,  Env * env){
  SDL_Surface * surf;
  SDL_Color color_menthe = { 22,184,78,255 }; //couleur menthe
  SDL_Color color_corail = { 231,62,1,255 }; //couleur corail
  node n = game_node(env->g, node_num);
  TTF_Font * font = TTF_OpenFont(FONT, env->fontsize);
  if(!font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD);
  char * degree = malloc(sizeof(char)*10);
  sprintf(degree, "%d", get_required_degree(n));
  if(get_required_degree(game_node(env->g, node_num)) == get_degree(env->g, node_num))
    surf = TTF_RenderText_Blended(font, degree , color_menthe);
  else
    surf = TTF_RenderText_Blended(font, degree , color_corail);
  env->text[node_num] = SDL_CreateTextureFromSurface(ren, surf);
  SDL_FreeSurface(surf);
  TTF_CloseFont(font);
}

/*
  Initialisation des variables d'env en fonction de la taille de l'écran
*/
void init_window(int w, int h, SDL_Renderer* ren, Env * env){
  int max_x = 0; int max_y = 0;
  int margin_x = 0; int margin_y = 0;

  // recupération de max_x et max_y
  for(int i = 0 ; i < game_nb_nodes(env->g) ; i++){
    node n = game_node(env->g, i);
    if(get_x(n) > max_x) max_x = get_x(n);
    if(get_y(n) > max_y) max_y = get_y(n);
  }
  env->max_x = max_x + 1; env->max_y = max_y + 1;

// definiton de la marge
  if(env->max_x * h > env->max_y * w){
    env->size = w /(env->max_x * 2);
    margin_y = (h  - env->size * (env->max_y * 2)) / 2;
  }
  else {
    env->size = h /(env->max_y * 2);
    margin_x = (w  - env->size * (env->max_x * 2)) / 2;
  }
  env->margin_x = margin_x; env->margin_y = margin_y;

  // definition de la taille de la police
  env->fontsize = env->size/6;
}

static int coordtopxx(int coord, Env * env){
  return (coord * 2 + 1) * env->size  - env->size / 2 + env->margin_x;
}

static int coordtopxy(int coord, Env * env){
  return (coord * 2 + 1) * env->size  - env->size / 2 + env->margin_y;
}

Env * init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]) {

  char * game_file = NULL;
  game g = NULL;

  /* L'utilisateur a rentré un nom de fichier */
  if(argc == 2) game_file = argv[1];
  if (game_file != NULL) printf("%s\n",game_file); //debug

  /* On charge le game en fonction de ce que l'utilisateur demande*/
  if(game_file != NULL) {g = translate_game(game_file); }
  else {g = translate_game("save/game_default.txt"); }

  /* test retour fonction translate*/
  if (g == NULL) return NULL;

  /*allocation de notre structure d'environnement*/
  Env * env = malloc(sizeof(struct Env_t));
  if(env == NULL) {delete_game(g); return NULL;} //ERREUR

  env->g = g;
  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  init_window(w,h,ren,env);

  /* Init island texture from PNG image */
  env->island=malloc(sizeof(SDL_Texture*)*game_nb_nodes(g));
  if(env->island == NULL) exit(EXIT_FAILURE); //ERREUR
  env->islandnonselect = IMG_LoadTexture(ren, ISLAND);
  if(!env->island) ERROR("IMG_LoadTexture: %s\n", ISLAND); //ERREUR

  for(int i = 0 ; i < game_nb_nodes(g) ; i++){
     env->island[i] = env->islandnonselect;
  }

  /* Init boat texture from PNG image */
  env->boat1 = IMG_LoadTexture(ren, BOAT1);
  if(!env->boat1) ERROR("IMG_LoadTexture: %s\n", BOAT1); //ERREUR
  env->boat2 = IMG_LoadTexture(ren, BOAT2);
  if(!env->boat2) ERROR("IMG_LoadTexture: %s\n", BOAT2); //ERREUR
  env->boat3 = IMG_LoadTexture(ren, BOAT3);
  if(!env->boat3) ERROR("IMG_LoadTexture: %s\n", BOAT3); //ERREUR
  env->boat4 = IMG_LoadTexture(ren, BOAT4);
  if(!env->boat4) ERROR("IMG_LoadTexture: %s\n", BOAT4); //ERREUR
  env->islandselect = IMG_LoadTexture(ren,ISLANDSELECT);

  SDL_Texture ** text = malloc(sizeof(SDL_Texture*)*game_nb_nodes(g));

  if(text == NULL) {delete_game(g); free(env); return NULL;} //ERREUR //SDLdeletetexture env->island

  env->text = text;

  for(int i = 0 ; i < game_nb_nodes(g) ; i++){
    print_degree(i, ren, env);
  }
  env->node = -1;
  return env;
}

void render(SDL_Window* win, SDL_Renderer* ren, Env * env) {

   SDL_Rect rect;

   int width, height;

   SDL_GetWindowSize(win, &width, &height);


   SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);

   int x,y,x1,y1;

   /* nodes */
   for(int i = 0; i < game_nb_nodes(env->g); i++){
      node n = game_node(env->g, i);

      /* texture */
      rect.w = env->size;
      rect.h = env->size;
      rect.x = coordtopxx(get_x(n),env);
      rect.y = coordtopxy(get_y(n),env);
      SDL_RenderCopy(ren, env->island[i], NULL, &(rect));

      /* degree */
      SDL_QueryTexture(env->text[i], NULL, NULL, &rect.w, &rect.h);
      rect.x = coordtopxx(get_x(n),env) + env->size /1.5;
      rect.y = coordtopxy(get_y(n),env) + env->size /1.5;
      SDL_RenderCopy(ren, env->text[i], NULL, &rect);

      if(get_degree(env->g, i)){
         for(int d=0;d<game_nb_dir(env->g);d++){
            if(get_degree_dir(env->g, i, d)>0){
               node cible = game_node(env->g,get_neighbour_dir(env->g, i, d));
               switch(d){
                  default:
                     exit(EXIT_FAILURE); //ce cas ne peut pas arriver
                  case NORTH:
                     x = coordtopxx(get_x(n),env)+env->size/2;
                     y = coordtopxy(get_y(n),env)+env->size;
                     x1 = coordtopxx(get_x(cible),env)+env->size/2;
                     y1 =coordtopxy(get_y(cible),env);
                     break;
                  case WEST:
                     x = coordtopxx(get_x(n),env);
                     y = coordtopxy(get_y(n),env)+env->size/2;
                     x1 = coordtopxx(get_x(cible),env)+env->size;
                     y1 =coordtopxy(get_y(cible),env)+env->size/2;
                     break;
                  case SOUTH:
                     x = coordtopxx(get_x(n),env)+env->size/2;
                     y = coordtopxy(get_y(n),env);
                     x1 = coordtopxx(get_x(cible),env)+env->size/2;
                     y1 =coordtopxy(get_y(cible),env)+env->size;
                     break;
                  case EAST:
                     x = coordtopxx(get_x(n),env)+env->size;
                     y = coordtopxy(get_y(n),env)+env->size/2;
                     x1 = coordtopxx(get_x(cible),env);
                     y1 =coordtopxy(get_y(cible),env)+env->size/2;
                     break;
                  case NW:
                     x = coordtopxx(get_x(n),env);
                     y = coordtopxy(get_y(n),env)+env->size;
                     x1 = coordtopxx(get_x(cible),env)+env->size;
                     y1 =coordtopxy(get_y(cible),env);
                     break;
                  case SW:
                     x = coordtopxx(get_x(n),env);
                     y = coordtopxy(get_y(n),env);
                     x1 = coordtopxx(get_x(cible),env)+env->size;
                     y1 =coordtopxy(get_y(cible),env)+env->size;
                     break;
                  case SE:
                     x = coordtopxx(get_x(n),env)+env->size;
                     y = coordtopxy(get_y(n),env);
                     x1 = coordtopxx(get_x(cible),env);
                     y1 =coordtopxy(get_y(cible),env)+env->size;
                     break;
                  case NE:
                     x = coordtopxx(get_x(n),env)+env->size;
                     y = coordtopxy(get_y(n),env)+env->size;
                     x1 = coordtopxx(get_x(cible),env);
                     y1 =coordtopxy(get_y(cible),env);
                     break;
               }
               SDL_RenderDrawLine(ren, x, y, x1, y1);
            }
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
   }
}

int get_node(int x, int y, Env * env){
  for(int i = 0; i < game_nb_nodes(env->g); i++){
    node n = game_node(env->g, i);
    int coordX = coordtopxx(get_x(n), env);
    int coordY = coordtopxy(get_y(n), env);
    if(coordX < x && coordX+env->size > x && coordY < y && coordY+env->size > y){
      return i;
    }
  }
  return -1;
}

void make_connection(int node_num, SDL_Renderer * ren, Env * env){
  if(env->node != -1 && node_num != -1){
    for(int i = 0; i < game_nb_dir(env->g); i++){
      if(get_neighbour_dir(env->g, node_num, i) == env->node){
        if(can_add_bridge_dir(env->g, node_num, i)){
          add_bridge_dir(env->g, node_num, i);
          env->island[env->node] = env->islandnonselect;
          env->island[node_num]=env->islandnonselect;
          print_degree(node_num, ren, env);
        }
        else {
          while(get_degree_dir(env->g, node_num, i) != 0){
            env->island[get_neighbour_dir(env->g, node_num, i)] = env->islandnonselect;
            env->island[node_num]=env->islandnonselect;
            del_bridge_dir(env->g, node_num, i);
            print_degree(node_num, ren, env);
          }
        }
        print_degree(env->node, ren ,env);
        env->node = -1;
        break;
      }
    }

  }
}

bool process(SDL_Window* win, SDL_Renderer* ren, Env * env, SDL_Event * e) {
   int w, h;
   SDL_GetWindowSize(win, &w, &h);

  /* generic events */
  if (e->type == SDL_QUIT) {
    return true;
  }

  if(e->type == SDL_WINDOWEVENT){
    int width, height;
    switch (e->window.event) {
      case SDL_WINDOWEVENT_RESIZED:
        SDL_GetWindowSize(win, &width, &height);
        init_window(width,height,ren,env);
        for(int i = 0; i < game_nb_nodes(env->g); i++){
          print_degree(i, ren, env);
        }
        break;
    }
  }

  if(e->type == SDL_MOUSEBUTTONDOWN){
    SDL_Point mousedir;
    SDL_GetMouseState(&mousedir.x, &mousedir.y);
    int node_num = get_node(mousedir.x, mousedir.y, env);
    env->island[node_num] = env->islandselect;
    if(env->node == -1)
      env->node = node_num;
    else if(env->node == node_num){
       env->node = -1;
       env->island[node_num] = env->islandnonselect;
    }
    else
      make_connection(node_num, ren, env);
  }

  if(e->type == SDL_MOUSEBUTTONUP){
    SDL_Point mousedir;
    SDL_GetMouseState(&mousedir.x, &mousedir.y);
    int node_num = get_node(mousedir.x,mousedir.y, env);
    make_connection(node_num, ren, env);
  }

  return false;

   /* generic events */
   if (e->type == SDL_QUIT) {
      return true;
   }

   /* Android events */

#ifdef __ANDROID__
   else if (e->type == SDL_FINGERDOWN) {
      int node_num = get_node(tfinger.x, tfinger.y, env);
      if(env->node == -1)
         env->node = node_num;
      else
         make_connection(node_num, ren, env);
   }
   else if(e->type == SDL_FINGERUP){
      int node_num = get_node(tfinger.x,tfinger.y, env);
      make_connection(node_num, ren, env);
   }

  #else
   else if(e->type == SDL_MOUSEBUTTONDOWN){
      SDL_Point mousedir;
      SDL_GetMouseState(&mousedir.x, &mousedir.y);
      int node_num = get_node(mousedir.x, mousedir.y, env);
      if(env->node == -1)
         env->node = node_num;
      else
         make_connection(node_num, ren, env);
   }

   else if(e->type == SDL_MOUSEBUTTONUP){
      SDL_Point mousedir;
      SDL_GetMouseState(&mousedir.x, &mousedir.y);
      int node_num = get_node(mousedir.x,mousedir.y, env);
      make_connection(node_num, ren, env);
   }
   #endif

   return false;
}


void clean(SDL_Window* win, SDL_Renderer* ren, Env * env) {
  for(int i = 0 ; i < game_nb_nodes(env->g) ; i++){
    //il faut rajouter le cas de island modifié
    //SDL_DestroyTexture(env->island[0]);
  }
  SDL_DestroyTexture(env->boat1);
  SDL_DestroyTexture(env->boat2);
  SDL_DestroyTexture(env->boat3);
  SDL_DestroyTexture(env->boat4);
  delete_game(env->g);
  free(env->text);
  free(env);
}
