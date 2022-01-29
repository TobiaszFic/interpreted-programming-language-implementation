#include "../inc/token.h"
#include "../error/error.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//identifier lookup table
static const char T_HUMAN_IDTABLE[30][15] = {
	"T_INT",
	"T_FLOAT",
	"T_IDENTIFIER",
	"T_KEYWORD",
	"T_EQ",
	"T_PLUS",
	"T_MINUS",
	"T_MUL",
	"T_DIV",
	"T_MOD",
	"T_EQEQ",
	"T_NEQ",
	"T_GT",
	"T_GTE",
	"T_LT",
	"T_LTE",
	"T_NOT",
	"T_LPAREN",
	"T_RPAREN",
	"T_LBRACE",
	"T_RBRACE",
	"T_COMMA",
	"T_ARROW",
	"T_SEMI",
	"T_EOF"
};

//methods
static void token__show(struct token* token) {
	if(token->type == T_INT) {
		printf("%s:%d", T_HUMAN_IDTABLE[token->type], token->value.i);
		return;
	}
	if(token->type == T_FLOAT) {
		printf("%s:%f", T_HUMAN_IDTABLE[token->type], token->value.f);
		return;	
	}
	else {
		printf("%s", T_HUMAN_IDTABLE[token->type]);
	}
}

//constructors
struct token* token__init(enum tokenType initType, int initLineNumber, int initPos_start, int initPos_end) {
	struct token* token = (struct token*)calloc(1, sizeof(struct token));
	if(token == NULL) {
		error_memory("token.c", "token__init", "token");
	}

	token->type = initType;
	token->lineNumber = initLineNumber;
	token->pos_start = initPos_start;
	token->pos_end = initPos_end;
	
	token->show = &token__show;
	return token;
}

struct token* token__init__copy(const struct token* initToken) {
	struct token* token = (struct token*)calloc(1, sizeof(struct token));
	if(token == NULL) {
		error_memory("token.c", "token__init__copy", "token");
	}

	token->type = initToken->type;
	token->lineNumber = initToken->lineNumber;
	token->pos_start = initToken->pos_start;
	token->pos_end = initToken->pos_end;

	
	token->show = &token__show;
	return token;
}

// destructor 
void token__cleanup(struct token* token) {
	free(token);
}

