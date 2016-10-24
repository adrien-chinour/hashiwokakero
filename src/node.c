#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct node_s {
	int x;
	int y;
	int required_degree;
} *node;

node new_node (int x, int y, int required_degree){
	node n = (node) malloc(sizeof(node));
	n->x = x;
	n->y = y;
	n->required_degree = required_degree;
	return n;
}

void delete_node(node n){
	free(n);
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
