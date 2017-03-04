#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

struct SList{
  data_type data;
  struct SList* next;
};

SList
slist_create_empty(void){
    return NULL;
}

SList
slist_alloc(void){
  SList p = (SList) malloc( sizeof(struct SList) );
  assert(p);
  p->next = NULL;
  return p;
}

void
slist_free_link(SList link_){
    if(link_ != NULL)
        free(link_);
}

SList 
slist_prepend(SList L, data_type data){
    SList p = slist_alloc();
    assert(p);
    p->data = data;
    p->next = L;
    return p;
}

SList 
slist_delete_first(SList L){
    if(L != NULL){
        SList next= L->next;
        slist_free_link(L);
        L= next;
    }
    return L;
}

SList 
slist_insert_after(SList L, SList p, data_type data){
    if(p != NULL){
        SList new = slist_alloc();
        new->data = data;
        new->next = p->next;
        p->next= new;
    }
    return L;
}

SList 
slist_delete_after(SList L, SList p){
    if(p != NULL){
        if(p->next != NULL){
            SList to_be_deleted = p->next;
            p->next = to_be_deleted->next;
            slist_free_link(to_be_deleted);
        }
    }
    return L;
}

SList  
slist_next(SList L){
    assert(L);
    return L->next;
}

data_type
slist_data(SList L){
    assert(L);
    return L->data;
}

bool
slist_isEmpty(SList L){
    return (L == NULL);
}
