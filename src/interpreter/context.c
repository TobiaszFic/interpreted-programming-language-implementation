#include "context.h"
#include "../error/error.h"
#include <stdlib.h>
#include "number.h"
#include "string.h"

struct symbolTable* symbolTable__set(struct context* context, struct token* token, struct number* number) {
	for(int i = 0; i< context->symbolTable->numOfSymbols; i++)
		if(!strcmp(context->symbolTable->items.symbols[i]->value.id_str, token->value.id_str)) {
			context->symbolTable->items.numbers[i] = number;
	}
	if(context->parent!=NULL)
		return symbolTable__set(context->parent, token, number);
	return NULL;
}
struct symbolTable* symbolTable__add(struct context* context, struct token* token, struct number* number) {
	struct symbolTable* foundVar = symbolTable__set(context, token, number);
	if(foundVar != NULL) {
		return foundVar;
	}
	for(int i = 0; i< context->symbolTable->numOfSymbols; i++)
		if(context->symbolTable->items.symbols[i] == NULL && context->symbolTable->items.symbols[i] == NULL) {
			context->symbolTable->items.numbers[i] = number;
			context->symbolTable->items.symbols[i] = token;
			return context->symbolTable;
	}
	context->symbolTable->items.symbols = realloc(context->symbolTable->items.symbols, (context->symbolTable->numOfSymbols+3)*sizeof(struct token*));
	context->symbolTable->items.numbers = realloc(context->symbolTable->items.numbers, (context->symbolTable->numOfSymbols+3)*sizeof(struct numbers*));
	context->symbolTable->items.symbols[context->symbolTable->numOfSymbols] = token;
	context->symbolTable->items.numbers[context->symbolTable->numOfSymbols] = number;
	context->symbolTable->numOfSymbols += 1; 
	return context->symbolTable;
}

void symbolTable__remove(struct symbolTable* table, struct token* token){
	for(int i = 0; i<table->numOfSymbols; i++)
		if(table->items.symbols[i] == token){
			table->items.numbers[i] = NULL;
			table->items.symbols[i] = NULL;
		}
}

struct symbolTable* symbolTable__setFunc(struct context* context, struct node* node) {
	for(int i = 0; i< context->symbolTable->numOfSymbols; i++)
		if(!strcmp(context->symbolTable->functions.symbols[i]->exp.funcDef.name->value.id_str, node->exp.funcDef.name->value.id_str)) {
			error_runtime("Redeclaration of function", context, node);
	}
	if(context->parent!=NULL)
		return symbolTable__setFunc(context->parent, node);
	return NULL;
}

struct symbolTable* symbolTable__addFunc(struct context* context, struct node* node) {
	struct symbolTable* foundFunc = symbolTable__setFunc(context, node);
	if(foundFunc != NULL) {
		error_runtime("Redeclaration of function", context, node);
	}
	context->symbolTable->functions.symbols = realloc(context->symbolTable->functions.symbols, (context->symbolTable->numOfFunctions+3)*sizeof(struct node*));
	context->symbolTable->functions.symbols[context->symbolTable->numOfFunctions] = node;
	context->symbolTable->numOfFunctions += 1; 
	return context->symbolTable;
}

struct node* symbolTable__getFunc(struct context* context, struct node* node) {
	for(int i = 0; i<context->symbolTable->numOfFunctions; i++){
		if(!strcmp(context->symbolTable->functions.symbols[i]->exp.funcDef.name->value.id_str, node->exp.funcCall.name->value.id_str))
			return context->symbolTable->functions.symbols[i];	
	}
	if(context->parent != NULL) {
		return symbolTable__getFunc(context->parent, node);
	}
	error_runtime("function uninitialized", context, node);
	return NULL;
}

struct number* symbolTable__get(struct context* context, struct token* token, struct node* node) {
	for(int i = 0; i<context->symbolTable->numOfSymbols; i++){
		if(!strcmp(context->symbolTable->items.symbols[i]->value.id_str, token->value.id_str))
			return context->symbolTable->items.numbers[i];	
	}
	if(context->parent != NULL) {
		return context->parent->symbolTable->get(context->parent, token, node);
	}
	error_runtime("variable uninitialized", context, node);
	return NULL;
}

struct symbolTable* symbolTable__init() {
	struct symbolTable* table	= calloc(1, sizeof(struct symbolTable));
	table->items.symbols		= calloc(3, sizeof(struct token*));
	table->items.numbers		= calloc(3, sizeof(struct numbers*));
	table->functions.symbols	= calloc(3, sizeof(struct node*));
	table->numOfSymbols = 0;

	table->add = &symbolTable__add; 
	table->get = &symbolTable__get;
	table->addFunc = &symbolTable__addFunc; 
	table->getFunc = &symbolTable__getFunc;

	return table;
}
struct context* context__init (char* displayName, struct context* parent, int parentEntryPos) {
	struct context* context = calloc(1, sizeof(struct context));
	context->symbolTable = symbolTable__init();
	context->displayName = displayName;
	context->parent = parent;
	context->parentEntryPos = parentEntryPos;

	return context;
}
