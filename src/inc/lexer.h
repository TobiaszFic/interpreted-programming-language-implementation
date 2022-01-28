#ifndef LEXER_H
#define LEXER_H
#include "token.h"

#include <stdio.h>

struct tokenList {
	struct token** items; //list of tokens
	int length;
};

struct lexer {
	//parameters
	char** lines;
	char* text;
	int position;
	char c;
	unsigned int lineNumber;
	FILE* stream;
	
	struct tokenList tokenList;

	//methods	
	void (*show)(struct lexer*);
	void (*advance)(struct lexer*);
	void (*lex)(struct lexer*);
	void (*getline)(struct lexer*);
}; 


struct lexer* lexer__init(FILE* initStream);//constructor
void lexer__cleanup(struct lexer* lexer);//destructor

#endif
