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

struct parser {
	//parameters
	struct tokenList tokenList; //list of tokens - input
	struct node* AST;//abstract syntax tree root - output of the parser and input of the interpreter

	int position;//within token list
	struct token* currentToken;

	struct lexer* parentLexer;

	//methods
	struct token*	(*advance)	(struct parser*);
	struct node*	(*factor)	(struct parser*);
	struct node*	(*term)		(struct parser*);
	struct node*	(*arit)		(struct parser*);
	struct node*	(*exp)		(struct parser*);
	struct node*	(*comp)		(struct parser*);
	struct node*	(*parse)	(struct parser*);
};

struct parser* parser__init(struct lexer* initLexer);
void parser__cleanup(struct parser* parser);
#endif
