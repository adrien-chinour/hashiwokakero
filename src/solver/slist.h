#ifndef ASDE_SLIST_H
#define ASDE_SLIST_H

#include <stdbool.h>

typedef int data_type;
typedef struct SList *SList;

// returns an empty list
extern SList slist_create_empty(void);

// allocates space for one SList element -> fonction creerCellule(): liste d'objet;
extern SList slist_alloc(void);

// frees space for one SList element -> fonction libererCellule(ref L:liste d'objet): vide;
extern  void slist_free_link(SList link_); 

//adds a new element on the start of the list -> fonction insereEnTete(val L: liste d'objet, val x objet): liste d'objet;
extern SList slist_prepend(SList L, data_type data);

// -> fonction supprimerEnTete(val L: liste d'objet): liste d'objet;
extern SList slist_delete_first(SList L);

// -> fonction insererApres(val L:liste d'objet, val p : liste d'objets, val x: objet): liste d'objet;
extern SList slist_insert_after(SList L, SList p, data_type data);

// -> fonction supprimerApres(val L:liste d'objet, val p : liste d'objets): liste d'objet;
extern SList slist_delete_after(SList L, SList p);

// gets next element in a SList ->  fonction suivant(val L:liste d'objet): liste d'objet; 
extern  SList  slist_next(SList L);

// gets data in a SList ->  fonction valeur(val L:liste d'objet): objet;
extern  data_type slist_data(SList L);

// tests whether a SList is empty
extern  bool slist_isEmpty(SList L);

#endif /* ASDE_SLIST_H */ 
