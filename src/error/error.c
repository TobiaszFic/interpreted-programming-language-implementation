#include "error.h"
#include "../shell/shell.h"

#include <stdio.h>
#include <stdlib.h>


static void printTroubledCharsFromNode( struct node* node) {
	struct lexer* lexer = node->parentLexer;
	printf("\n\t\t%s\n\t\t",lexer->lines[node->parentToken->lineNumber-1]);
	for(int i = 0; i<node->pos_start; i++)
		putchar(' ');
	int localPos = node->pos_start;
	while(localPos < node->pos_end) {
		putchar('^');
		localPos++;
	}
	fflush(stdout);
}

static void printTroubledChars( struct lexer* lexer, struct token* token) {
	printf("\n\t\t%s\n\t\t",lexer->lines[token->lineNumber-1]);
	for(int i = 0; i<token->pos_start; i++)
		putchar(' ');
	int localPos = token->pos_start;
	while(localPos < token->pos_end) {
		putchar('^');
		localPos++;
	}
}

void error__(FILE* fp) {
	if(fp == stdin) {
		launchShell();		
	} else{
		exit(1);
	}	
}

void error_lexer(char* eMessage, struct lexer* eLexer) {
	printf("\n Lexer Error: %s", eMessage);
	printf("\n\tline %d :", eLexer->lineNumber);
	printf("%s", eLexer->lines[eLexer->lineNumber-1]);
	printTroubledChars(eLexer, eLexer->tokenList.items[eLexer->tokenList.length-1]);
	error__(eLexer->stream);	
}

void error_parser(char* eMessage, struct parser* eParser) {
	printf("\n Parser Error: %s", eMessage);
	printf("\n\tline %d :", eParser->currentToken->lineNumber);
	printTroubledChars(eParser->parentLexer, eParser->currentToken);
	error__(eParser->parentLexer->stream);	
}

void error_memory(char* fileName, char* functionName, char* varName) {
	printf("\nMemory Error: error allocating memory for item \"%s\" in function \"%s\"\nsource file: %s\n", varName, fileName, functionName);	
	exit(1);
}

static void printContext(struct context* context) {
	printf("\n\t\t<%s>", context->displayName);
	if(context->parent != NULL)
		printContext(context->parent);
}

void error_runtime(char* eMessage, struct context* context, struct node* node) {
	printf("\nRuntime Error:");
	printf("\n\tline %d :", node->parentToken->lineNumber);
	puts(eMessage);
	printTroubledCharsFromNode(node);
	printf("\n\ttraceback:");
	printContext(context);
	putchar('\n');
	error__(node->parentLexer->stream);	
}

void warning_lexer(char* eMessage, struct lexer* eLexer) {
	printf("\n Lexer Warning: %s", eMessage);
	printf("\n\tline %d :", eLexer->lineNumber);
	printTroubledChars(eLexer, eLexer->tokenList.items[eLexer->tokenList.length-1]);
	putchar('\n');
	fflush(stdout);
}

void warning_parser(char* eMessage, struct parser* eParser) {
	printf("\n Parser Warning: %s", eMessage);
	printf("\n\tline %d :", eParser->currentToken->lineNumber);
	printTroubledChars(eParser->parentLexer, eParser->currentToken);
	putchar('\n');
	fflush(stdout);
}

