#include "../inc/lexer.h"
#include "../inc/token.h"
#include "../error/error.h"
#include "lexer_add.h"

//#include <stdio.h> - already included in lexer.h
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Nota bene:
//
//- Whenever you see an instance of the LEXER struct passed into a function/method,
//think "self" operator in c++, remember this is c so there's no direct method-instance access.
//Tried using "thisLexer" as a name but that got very messy very quickly
//
//- Passing the pointer to the list of tokens into a function only to return it back and reassign it to the same
//list of tokens may seem like a mistake, but because of the way realloc works, it can't be done otherwise.
//
//- Error handling bloats the file but needs to be done.


//helper functions (included in the lexer_add.c file):
//-addSimpleToken
//-addNumber


//methods
static void lexer__advance(struct lexer* lexer){
	lexer->position += 1;
	lexer->c = lexer->text[lexer->position];
}

static void lexer__lex(struct lexer* lexer) {
	lexer->tokenList.length = 0;
	lexer->tokenList.items = (struct token**)calloc(1,sizeof(struct token*));
	if (lexer->tokenList.items == NULL) {
		error_memory("src/lexer/lexer.c", "lexer__lex", "lexer->tokenList.items");
	}
	
	while (lexer->c != EOF) {
		/*
		switch (lexer->c) {
			case '+':
				lexer->tokenList.items = addSimpleToken(lexer, T_PLUS);
				break;
			case '-':
				lexer->tokenList.items = addSimpleToken(lexer, T_MINUS);
				break;
			case '*':
				lexer->tokenList.items = addSimpleToken(lexer, T_MUL);
				break;
			case '/':
				lexer->tokenList.items = addSimpleToken(lexer, T_DIV);
				break;
			case '(':
				lexer->tokenList.items = addSimpleToken(lexer, T_LPAREN);
				break;
			case ')':
				lexer->tokenList.items = addSimpleToken(lexer, T_RPAREN);
				break;
			default:
				if(lexer->c >= 48 && lexer->c <= 57) {
					lexer->tokenList.items = addNumber(lexer);
					break;
				} else if(lexer->c == '\t' || lexer->c == ' ' || lexer->c == '\n' || lexer->c == '\0'|| lexer->c == '\r') {
					lexer->advance(lexer);
					break;
				} 
				printf("%u", lexer->c);
				putchar(lexer->c);
				warning_lexer("Illegal character", lexer);
				lexer->advance(lexer);
				break;
		}
		*/
		if(lexer->c == '\0')
			lexer->getline(lexer);
		else if(lexer->c == '\t' || lexer->c == ' ' || lexer->c == '\n')
			lexer->advance(lexer);
		else if(lexer->c == '+')
			lexer->tokenList.items = addSimpleToken(lexer, T_PLUS);
		else if(lexer->c == '*')
			lexer->tokenList.items = addSimpleToken(lexer, T_MUL);
		else if(lexer->c == '/')
			lexer->tokenList.items = addSimpleToken(lexer, T_DIV);
		else if(lexer->c == '(')
			lexer->tokenList.items = addSimpleToken(lexer, T_LPAREN);
		else if(lexer->c == ')')
			lexer->tokenList.items = addSimpleToken(lexer, T_RPAREN);
		else if(lexer->c == ',')
			lexer->tokenList.items = addSimpleToken(lexer, T_COMMA);
		else if(lexer->c == '=')
			lexer->tokenList.items = makeEquals(lexer);
		else if(lexer->c == '!')
			lexer->tokenList.items = makeNotEquals(lexer);
		else if(lexer->c == '>')
			lexer->tokenList.items = makeGreater(lexer);
		else if(lexer->c == '<')
			lexer->tokenList.items = makeLesser(lexer);
		else if(lexer->c == '-')
			lexer->tokenList.items = makeArrow(lexer); //returns a minus when doesnt find arrow
		else if(lexer->c >= 48 && lexer->c <= 57)
			lexer->tokenList.items = addNumber(lexer);
		else if((lexer->c >= 65 && lexer->c <= 90) ||(lexer->c >= 97 && lexer->c <= 122))
			lexer->tokenList.items = makeIdentifier(lexer);
		else {
			warning_lexer("Illegal character", lexer);
			lexer->advance(lexer);
		}
		

	}
	lexer->tokenList.items = addSimpleToken(lexer, T_EOF);

	//lexer->show(lexer);

}

static void lexer__getline(struct lexer* lexer) {
	lexer->position = -1;
	char* line;
    lexer->lineNumber++;
    FILE* fp = lexer->stream;
    
    line = (char*)calloc(1, sizeof(char));
    if(line == NULL) {
		error_memory("src/lexer/lexer.c", "lexer__getline", "line");
    }
	lexer->lines = (char**)realloc(lexer->lines, (lexer->lineNumber + 2)*sizeof(char*));
	if(lexer->lines == NULL) {
		error_memory("src/lexer/lexer.c", "lexer__getline", "lexer->lines");
	}
    
    int i = 0;    
    char c;
	bool endFlag = false;

    while(endFlag == false) {
        c = fgetc(fp);

        line = (char*)realloc(line, (i+2)*sizeof(char));
		if(line == NULL) {
			error_memory("src/lexer/lexer.c", "lexer__getline", "line");
        }

        switch(c) {
            case EOF:
                line[i] = EOF;
				endFlag = true;
                break;
            case '#': //for shell use only
                if(lexer->stream == stdin) {
                	line[i] = EOF;
					endFlag = true;
                	break;
				}
            case '\n':
                line[i] = '\0';
				endFlag = true;
                break;
            default:
                line[i] = c;
                i++;
        }
    }
	lexer->lines[lexer->lineNumber - 1] = line;

	lexer->text = line;
	lexer->advance(lexer);
}

static void lexer__tokens__show (struct lexer* lexer) {
	printf("\nLEXER: [");
	for(int i = 0; i<lexer->tokenList.length; i++) {
		lexer->tokenList.items[i]->show(lexer->tokenList.items[i]);//looks fucky but just a self-reference
		if(i != (lexer->tokenList.length-1))
			printf(", ");
	}
	printf("]\n");
}

//constructor
struct lexer* lexer__init(FILE* initStream) {
	struct lexer* lexer = (struct lexer*)calloc(1, sizeof(struct lexer));
	lexer->lines = (char**)calloc(1, sizeof(char*));
	lexer->text = NULL;
	lexer->position = -1;
	lexer->c = ' ';
	lexer->lineNumber = 0;
	lexer->stream = initStream;

	lexer->tokenList.items = NULL;
	lexer->tokenList.length = 0;

	lexer->advance = &lexer__advance;
	lexer->lex = &lexer__lex;
	lexer->getline = lexer__getline;
	lexer->show = lexer__tokens__show;

	lexer->getline(lexer);//initializing text array, includes an advance
	return lexer;
}

//destructor
void lexer__cleanup(struct lexer* lexer) {
	//tokenList cleanup - we can keep the rest of the lexer until we don't need the list
	//anymore, since all the other items take up very little space.
	for(int i = 0; i <= lexer->tokenList.length; i++)
		free(lexer->tokenList.items[i]);
	free(lexer->tokenList.items);

	free(lexer->text);
	free(lexer);
}

