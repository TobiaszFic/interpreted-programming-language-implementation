#ifndef PARSER_H
#define PARSER_H
#include "node.h"
#include "lexer.h"
#include "token.h"

/*
	just like the tokenizer, the parser analyzes linearly,
however it advances across tokens, not characters.
*/

/* 
 * reminder:
	struct tokenList {
		struct token* tokens; //list of tokens
		int length;
	}tokenList;
*/
struct nodeList {
	struct node** items;
	int len;

	struct nodeList* (*append)(struct nodeList* nodes, struct node* item);
	void (*show)(struct nodeList* nodes);
};

struct nodeList* nodeList__init();

struct parser {
	//parameters
	struct tokenList tokenList; //list of tokens - input
	struct nodeList* AST;//abstract syntax tree root - output of the parser and input of the interpreter

	int position;//within token list
	struct token* currentToken;

	struct lexer* parentLexer;

	//methods
	struct token*	(*advance)	(struct parser*);
	struct nodeList*	(*parse)	(struct parser*);
};

struct nodeList* parser__statements(struct parser* parser);
struct parser* parser__init(struct lexer* initLexer);
void parser__cleanup(struct parser* parser);
#endif
