
#ifndef CONTEXT_H
#define CONTEXT_H
#include "number.h"
struct context;
struct symbolTable {
	struct{
		struct token** symbols;
		struct number** numbers;
	} items;
	struct {
		struct node** symbols;
	} functions;
	int numOfSymbols;
	int numOfFunctions;
	struct symbolTable* parent;
	//methods
	struct symbolTable* (*add)(struct context* context, struct token* token, struct number* value); 
	struct number* (*get)(struct context* context, struct token* token, struct node* node);
	struct symbolTable* (*addFunc)(struct context* context, struct node* node); 
	struct node* (*getFunc)(struct context* context, struct node* node);
};
struct symbolTable* symbolTable__init();

struct context {
	struct symbolTable* symbolTable;
	char* displayName;
	struct context* parent;
	int parentEntryPos;
};
struct context* context__init (char* displayName, struct context* parent, int parentEntryPos);
#endif
