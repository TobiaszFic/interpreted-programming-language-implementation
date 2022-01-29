#include "../inc/parser.h"
#include<stdlib.h>
#include<stdio.h>




struct nodeList* nodeList__append(struct nodeList* nodes, struct node* item) {
	nodes->items = realloc(nodes->items, (nodes->len + 3)*sizeof(struct node*));
	nodes->items[nodes->len] = item;
	nodes->len += 1;
	return nodes;
}

void nodeList__show(struct nodeList* nodes) {
	for(int i = 0; i < nodes->len; i++) {
		putchar('\n');
		nodes->items[i]->show(nodes->items[i]);
	}
}

struct nodeList* nodeList__init() {
	struct nodeList* nodes = calloc(1, sizeof(struct nodeList));
	nodes->items = calloc(2, sizeof(struct node*));
	nodes->len = 0;

	nodes->append = &nodeList__append;
	nodes->show = &nodeList__show;
	return nodes;	
}



