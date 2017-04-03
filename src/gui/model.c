#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "model.h"
#include "../core/game.h"
#include "../core/node.h"
#include "../tools/file.h"
#include "../tools/generate.h"

/* **************************************************************** */

#define FONT "Luna.ttf"
#define ISLAND "img/island.png"
#define BOAT1 "img/boat1.png"
#define BOAT2 "img/boat2.png"
#define BOAT3 "img/boat3.png"
#define BOAT4 "img/boat4.png"
#define ISLANDSELECT "img/island_selected.png"
#define SOLVE "img/solve.png"
#define SAVE "img/save.png"

/* **************************************************************** */

struct Env_t {
  SDL_Texture ** island; // texture ile
  SDL_Texture ** text; // texture texte (degré)
  SDL_Texture * islandnonselect;
  SDL_Texture * islandselect;
  int fontsize; // taille de la police (degré)
  int max_x; int max_y; // coordonnees maxmum des iles
  int margin_x; int margin_y; // marge pour centrer la partie
  game g; // la partie a afficher
  int node; // le noeud selectionné (evenement)
  int size; // taille d'une ile
  SDL_Texture * game_win;
  SDL_Texture * solve;
   SDL_Texture * save;
   SDL_Point mouse_pos;
};


/*
  Initialisation des variables d'env en fonction de la taille de l'écran
*/
void print_degree(int node_num, SDL_Renderer* ren,  Env * env){
  SDL_Surface * surf;
  SDL_Color color_menthe = { 22,184,78,255 }; //couleur menthe
  SDL_Color color_corail = { 231,62,1,255 }; //couleur corail
  node n = game_node(env->g, node_num);
  TTF_Font * font = TTF_OpenFont(FONT, env->fontsize);
  if(!font) SDL_Log("TTF_OpenFont: %s\n", FONT);
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
  env->max_x = max_x + 1; env->max_y = max_y + 2;

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



  //il y a une erreur sur le chargement de la map sur android

  #ifdef __ANDROID__
    // generation aleatoire d'ue partie 3*3 à 9 noeud avec nb_dir = 4 et max_bridges = 2
    g = random_game(2,4);
  #else
    /* On charge le game en fonction de ce que l'utilisateur demande*/
    if(game_file != NULL) {g = translate_game(game_file); }
    else {g = translate_game("save/game_default.txt"); }
  #endif

  /* test retour fonction translate*/
  if (g == NULL) return NULL;


  /*allocation de notre structure d'environnement*/
  Env * env = malloc(sizeof(struct Env_t));
  if(env == NULL) {delete_game(g); return NULL;} //ERREUR

  env->game_win = NULL;
  env->g = g;

  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  init_window(w,h,ren,env);

  /* Init island texture from PNG image */
  env->island=malloc(sizeof(SDL_Texture*)*game_nb_nodes(g));
  if(env->island == NULL) exit(EXIT_FAILURE); //ERREUR
  env->islandnonselect = IMG_LoadTexture(ren, ISLAND);
  SDL_SetTextureBlendMode(env->islandnonselect,SDL_BLENDMODE_BLEND);
  if(!env->island) SDL_Log("IMG_LoadTexture: %s\n", ISLAND); //ERREUR

  env->solve = IMG_LoadTexture(ren, SOLVE);
  if(!env->solve) SDL_Log("IMG_LoadTexture: %s\n", SOLVE);

  env->save = IMG_LoadTexture(ren, SAVE);
  if(!env->save) SDL_Log("IMG_LoadTexture: %s\n", SAVE);

  for(int i = 0 ; i < game_nb_nodes(g) ; i++){
    env->island[i] = env->islandnonselect;
  }


  env->islandselect = IMG_LoadTexture(ren,ISLANDSELECT);
  SDL_SetTextureBlendMode(env->islandselect,SDL_BLENDMODE_BLEND);

  SDL_Texture ** text = malloc(sizeof(SDL_Texture*)*game_nb_nodes(g));

  if(text == NULL) {delete_game(g); free(env); return NULL;} //ERREUR //SDLdeletetexture env->island

  env->text = text;

  for(int i = 0 ; i < game_nb_nodes(g) ; i++){
    node n = game_node(g, i);
    SDL_Color color = { 231, 62, 1, 255 };
    TTF_Font * font = TTF_OpenFont(FONT, env->fontsize);
    if(!font) SDL_Log("TTF_OpenFont: %s\n", FONT);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    char * degree = malloc(sizeof(char)*10);
    sprintf(degree, "%d", get_required_degree(n));
    SDL_Surface * surf = TTF_RenderText_Blended(font, degree , color);// blended rendering for ultra nice text
    //free(degree);
    env->text[i] = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
  }

  env->node = -1;

  return env;
}

void render(SDL_Window* win, SDL_Renderer* ren, Env * env) {

   SDL_Rect rect;

   int width, height;

   SDL_GetWindowSize(win, &width, &height);


   SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
   SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_ADD);


   int node_pos = get_node(env->mouse_pos.x,env->mouse_pos.y,env);
   int x =0;int y =0;int x1 =0;int y1 =0;

   if(node_pos!=-1){
      SDL_SetRenderDrawColor(ren,100,100,100,SDL_ALPHA_OPAQUE);
      for(int d =0; d<game_nb_dir(env->g); d++){
         if(can_add_bridge_dir(env->g,node_pos, d)){
            node debut = game_node(env->g,node_pos);
            node cible = game_node(env->g,get_neighbour_dir(env->g, node_pos, d));
            switch(d){
               case NORTH:
                  x = coordtopxx(get_x(debut),env)+env->size/2;
                  y = coordtopxy(get_y(debut),env)+env->size;
                  x1 = coordtopxx(get_x(cible),env)+env->size/2;
                  y1 =coordtopxy(get_y(cible),env);
                  rect.x = coordtopxx(get_x(debut),env)+env->size/4;
                  rect.y = coordtopxy(get_y(debut),env)+env->size;
                  rect.h = y1 -y;
                  rect.w = env->size/2;
                  break;
               case WEST:
                  x = coordtopxx(get_x(debut),env);
                  y = coordtopxy(get_y(debut),env)+env->size/2;
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 =coordtopxy(get_y(cible),env)+env->size/2;
                  rect.x = coordtopxx(get_x(debut),env);
                  rect.y = coordtopxy(get_y(debut),env) + env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 - x;
                  break;
               case SOUTH:
                  x = coordtopxx(get_x(debut),env)+env->size/2;
                  y = coordtopxy(get_y(debut),env);
                  x1 = coordtopxx(get_x(cible),env)+env->size/2;
                  y1 =coordtopxy(get_y(cible),env)+env->size;
                  rect.x = coordtopxx(get_x(debut),env)+env->size/4;
                  rect.y = coordtopxy(get_y(debut),env);
                  rect.h = y1 - y;
                  rect.w = env->size/2;
                  break;
               case EAST:
                  x = coordtopxx(get_x(debut),env)+env->size;
                  y = coordtopxy(get_y(debut),env)+env->size/2;
                  x1 = coordtopxx(get_x(cible),env);
                  y1 =coordtopxy(get_y(cible),env)+env->size/2;
                  rect.x = coordtopxx(get_x(debut),env)+env->size;
                  rect.y = coordtopxy(get_y(debut),env)+env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 -x;
                  break;
               case NW:
                  x = coordtopxx(get_x(debut),env);
                  y = coordtopxy(get_y(debut),env)+env->size;
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 =coordtopxy(get_y(cible),env);
                  rect.x = coordtopxx(get_x(debut),env)+env->size;
                  rect.y = coordtopxy(get_y(debut),env)+env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 -x;
                  break;
               case SW:
                  x = coordtopxx(get_x(debut),env);
                  y = coordtopxy(get_y(debut),env);
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 =coordtopxy(get_y(cible),env)+env->size;
                  rect.x = coordtopxx(get_x(debut),env)+env->size;
                  rect.y = coordtopxy(get_y(debut),env)+env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 -x;
                  break;
               case SE:
                  x = coordtopxx(get_x(debut),env)+env->size;
                  y = coordtopxy(get_y(debut),env);
                  x1 = coordtopxx(get_x(cible),env);
                  y1 =coordtopxy(get_y(cible),env)+env->size;
                  rect.x = coordtopxx(get_x(debut),env)+env->size;
                  rect.y = coordtopxy(get_y(debut),env)+env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 -x;
                  break;
               case NE:
                  x = coordtopxx(get_x(debut),env)+env->size;
                  y = coordtopxy(get_y(debut),env)+env->size;
                  x1 = coordtopxx(get_x(cible),env);
                  y1 =coordtopxy(get_y(cible),env);
                  rect.x = coordtopxx(get_x(debut),env)+env->size;
                  rect.y = coordtopxy(get_y(debut),env)+env->size/4;
                  rect.h = env->size/2;
                  rect.w = x1 -x;
                  break;
            }
            SDL_RenderFillRect(ren,&rect);
            SDL_RenderDrawRect(ren,&rect);
            SDL_RenderDrawLine(ren, x,y, x1, y1);
         }
      }
   }
   SDL_SetRenderDrawBlendMode(ren,SDL_BLENDMODE_MOD);

   print_node_and_bridges(env,ren);



      //timer


   SDL_Color color = {255, 203, 96, 255};
   TTF_Font * font = TTF_OpenFont(FONT, env->fontsize*2);
   if(!font) SDL_Log("TTF_OpenFont: %s\n", FONT);
   TTF_SetFontStyle(font, TTF_STYLE_BOLD);
   char * timer = malloc(sizeof(char)*100);
   int time = SDL_GetTicks();
   sprintf(timer, "Time : %d sec.", time/1000);
   SDL_Surface * surf = TTF_RenderText_Blended(font, timer , color);
   SDL_Texture * timer_texture = SDL_CreateTextureFromSurface(ren, surf);
   rect.w = env->size*2;
   rect.h = env->size;
   rect.x = width - rect.w;
   rect.y = height - rect.h;
   SDL_RenderCopy(ren, timer_texture, NULL, &(rect));
   free(timer);
   SDL_FreeSurface(surf);
   TTF_CloseFont(font);

      //solver
      rect.w = env->size/2;
      rect.h = env->size/2;
      rect.x = env->size/4;
      rect.y = height-env->size + env->size/4;
      SDL_RenderCopy(ren, env->solve, NULL, &(rect));

      //save
      rect.w = env->size/2;
      rect.h = env->size/2;
      rect.x = env->size;
      rect.y = height-env->size + env->size/4;
      SDL_RenderCopy(ren, env->save, NULL, &(rect));

   if(env->game_win){
      rect.w = env->size*6;
      rect.h = env->size*2;
      rect.x = width /2 - (rect.w / 2);
      rect.y = height /2 - (rect.h / 2);
      SDL_RenderCopy(ren, env->game_win, NULL, &(rect));
   }
}

static void game_finish(Env * env, SDL_Renderer * ren){ //note pour plus tard: pour avoir le dernier temps il faut mettre le temps dans la variable d'environnement ou en faire un variable globale
  if(game_over(env->g)){
    SDL_Color color = {255, 203, 96, 255};
    TTF_Font * font = TTF_OpenFont(FONT, env->fontsize*4);
    if(!font) ERROR("TTF_OpenFont: %s\n", FONT);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    //int last = time;
    //char * end_message = malloc(sizeof(char)*100);
    //sprintf(end_message, "Victoire ! Temps : %d sec.", last/1000);
    char * end_message = "Victoire !";
    SDL_Surface * surf = TTF_RenderText_Blended(font, end_message, color);
    SDL_Texture * end_texture = SDL_CreateTextureFromSurface(ren, surf);
    env->game_win = end_texture;
    //free(end_message);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
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
          env->island[node_num]=env->islandnonselect;
          print_degree(node_num, ren, env);
          game_finish(env,ren);
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
        env->island[env->node] = env->islandnonselect;
        env->node = -1;
        break;
      }
    }
  }
}


void print_node_and_bridges(Env * env, SDL_Renderer * ren){
   SDL_Rect rect;
   int x,y,x1,y1;
   int dx =0,dy=0;
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
               if(d == NORTH){
                  x = coordtopxx(get_x(n),env)+env->size/2;
                  y = coordtopxy(get_y(n),env)+env->size;
                  x1 = coordtopxx(get_x(cible),env)+env->size/2;
                  y1 =coordtopxy(get_y(cible),env);
                  dx =env->size/10;
                  dy =0;
                  if(can_add_bridge_dir(env->g, i, d)){
                     SDL_SetRenderDrawColor(ren, 0, 255, 0, SDL_ALPHA_OPAQUE);
                  }
                  else
                     SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
                  render_bridges(x,y,x1,y1,dx,dy,i,d,env,ren);
               }
               else if(d == WEST){
                  x = coordtopxx(get_x(n),env);
                  y = coordtopxy(get_y(n),env)+env->size/2;
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 =coordtopxy(get_y(cible),env)+env->size/2;
                  dx =0;
                  dy =env->size/10 ;
                  if(can_add_bridge_dir(env->g, i, d)){
                     SDL_SetRenderDrawColor(ren, 0, 255, 0, SDL_ALPHA_OPAQUE);
                  }
                  else
                     SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
                  render_bridges(x,y,x1,y1,dx,dy,i,d,env,ren);
               }
               else if(d == NW){
                  x = coordtopxx(get_x(n),env);
                  y = coordtopxy(get_y(n),env)+env->size;
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 =coordtopxy(get_y(cible),env);
                  dx =env->size/10;
                  dy =env->size/10 ;
                  if(can_add_bridge_dir(env->g, i, d)){
                     SDL_SetRenderDrawColor(ren, 0, 255, 0, SDL_ALPHA_OPAQUE);
                  }
                  else
                     SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
                  render_bridges(x,y,x1,y1,dx,dy,i,d,env,ren);
               }
               else if(d == SW){
                  x = coordtopxx(get_x(n),env);
                  y = coordtopxy(get_y(n),env);
                  x1 = coordtopxx(get_x(cible),env)+env->size;
                  y1 =coordtopxy(get_y(cible),env)+env->size;
                  dx =-env->size/10;
                  dy =env->size/10 ;
                  if(can_add_bridge_dir(env->g, i, d)){
                     SDL_SetRenderDrawColor(ren, 0, 255, 0, SDL_ALPHA_OPAQUE);
                  }
                  else
                     SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
                  render_bridges(x,y,x1,y1,dx,dy,i,d,env,ren);
               }
            }
         }
      }
   }
}

void render_bridges(int x, int y, int x1, int y1, int dx, int dy, int node_num,int dir, Env * env, SDL_Renderer * ren){
   int degree = get_degree_dir(env->g, node_num, dir);
   x = x+(dx*degree)/2;
   y = y+(dy*degree)/2;
   x1 = x1+(dx*degree)/2;
   y1 = y1+(dy*degree)/2;
   for(int j =0; j<get_degree_dir(env->g, node_num, dir);j++){
      SDL_RenderDrawLine(ren, x, y, x1, y1);
      x = x-dx;
      y = y-dy;
      x1 = x1-dx;
      y1 = y1-dy;
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

  /* Android events */

#ifdef __ANDROID__

  else if (e->type == SDL_FINGERDOWN) {
     int node_num = get_node(e->tfinger.x*w, e->tfinger.y*h, env);
     if(node_num !=-1){
        if(env->node == -1){
           env->island[node_num] = env->islandselect;
           env->node = node_num;
        }
        else if(env->node == node_num){
           env->node = -1;
           env->island[node_num] = env->islandnonselect;
        }
        else
           make_connection(node_num, ren, env);
     }
  }
  else if(e->type == SDL_FINGERUP){
     int node_num = get_node(e->tfinger.x*w,e->tfinger.y*h, env);
     make_connection(node_num, ren, env);
  }

  #else
  if(e->type == SDL_MOUSEBUTTONDOWN){
    SDL_Point mousedir;
    SDL_GetMouseState(&mousedir.x, &mousedir.y);
    int node_num = get_node(mousedir.x, mousedir.y, env);
    if(node_num !=-1){
      if(env->node == -1){
        env->island[node_num] = env->islandselect;
        env->node = node_num;
      }
      else if(env->node == node_num){
        env->node = -1;
        env->island[node_num] = env->islandnonselect;
      }
      else
        make_connection(node_num, ren, env);
    }
  }

  else if(e->type == SDL_MOUSEBUTTONUP){
    SDL_Point mousedir;
    SDL_GetMouseState(&mousedir.x, &mousedir.y);
    int node_num = get_node(mousedir.x,mousedir.y, env);
    make_connection(node_num, ren, env);
  }
  else if(e->type == SDL_MOUSEMOTION){
     SDL_GetMouseState(&env->mouse_pos.x, &env->mouse_pos.y);
  }
  #endif

  return false;
}

void clean(SDL_Window* win, SDL_Renderer* ren, Env * env) {
  for(int i = 0; i<game_nb_nodes(env->g);i++){
    SDL_DestroyTexture(env->text[i]);
  }
  SDL_DestroyTexture(env->islandnonselect);
  SDL_DestroyTexture(env->islandselect);
  delete_game(env->g);
  free(env->text);
  free(env);
}
