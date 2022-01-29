#include "../inc/parser.h"
#include "../inc/node.h"
#include "../inc/token.h"
#include "../error/error.h"

#include "grammar/grammar.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


static struct token* parser__advance(struct parser* parser) {
	if(parser->position >= parser->tokenList.length){
		putchar('a');fflush(stdout);
		return parser->currentToken;
	}
	parser->position += 1;
	parser->currentToken = parser->tokenList.items[parser->position];
	return parser->currentToken;
}

static struct nodeList* parser__parse (struct parser* parser) { //calls top-most node creation
	parser->AST = parser__statements(parser);
	if(parser->currentToken->type != T_EOF) {
		error_parser("expected '+', '-', '*' or '/'", parser);
	}
	printf("\nPARSER: ");
	fflush(stdout);
	parser->AST->show(parser->AST);
	fflush(stdout);
	return parser->AST;
}


//constructor
struct parser* parser__init(struct lexer* initLexer) {
	struct parser* parser = (struct parser*)calloc(1,sizeof(struct parser));
	parser->tokenList.items = initLexer->tokenList.items;
	parser->tokenList.length = initLexer->tokenList.length;
	//printf("\ntokenList length = %d\n", parser->tokenList.length);

	parser->position = -1;
	parser->currentToken = NULL;

	parser->parentLexer = initLexer;
 
	parser->advance = &parser__advance;
	parser->parse	= &parser__parse;

	parser->advance(parser);
	return parser;
}

void parser__cleanup(struct parser* parser) {
	free(parser); 
	//both the tokenList and the AST are passed onto the interpreter so we can't free them yet.
}
