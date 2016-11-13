#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node_s {
	int x;
	int y;
	int required_degree;
} *node;


node new_node(int x, int y, int required_degree){
	node n = (node) malloc(sizeof(struct node_s));
	// assert(new != NULL);
	n->x = x;
	n->y = y;
	n->required_degree = required_degree;
	return n;
}

void delete_node (node n){
	free(n);
	n = NULL;
}

int get_x (node n){
	return n->x;
}

int get_y (node n){
	return n->y;
}

int get_required_degree (node n){
	return n->required_degree;
}